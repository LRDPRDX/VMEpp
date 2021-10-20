#include <iostream>

#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>

#define LINUX

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"
#include "modules/V895.h"

using namespace vmepp;

void Histo()
{
    TCanvas* canvas = new TCanvas( "Test", "Test", 800, 800 );
    TH1F *hist = new TH1F( "V1190B", "V1190B", 100, 0, 100 );
        hist->Draw();

    V2718 controller;
    V1190B tdc( 0x20080000 );
    V895 disc( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &tdc );
        controller.RegisterSlave( &disc );
        controller.Initialize();

        disc.Enable( (1 << 14) | (1 << 0) );
        disc.WriteOutWidthH( 30 );
        disc.WriteOutWidthL( 20 );

        V2718::OutputConfig outCfg;
        outCfg.SOURCE = V2718::Src_t::PULS_SCAL;
        controller.WriteOutputConfig( V2718::Out_t::OUT0, outCfg );

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        pA->SetSquare( 10000, 1 );
        pA->SetNPulses( 0 );
        pA->SetStartSource( V2718::Src_t::SW );
        pA->SetStopSource( V2718::Src_t::SW );
        pA->Write();

        tdc.WriteDetection( V1190B::EdgeDetect_t::PAIR );
        V1190B::PairRes pRes;
            pRes.EDGE = V1190B::ResLeadEdgeTime::ps100;
            pRes.WIDTH = V1190B::ResPulseWidth::ps400;
        tdc.WritePairRes( pRes );

        tdc.WriteAcqMode( V1190B::TriggerMode_t::MATCHING );
        tdc.WriteWindowWidth( 20 ); // 500 ns
        tdc.WriteWindowOffset( -10 ); // -250 ns

        //tdc.WriteIRQEvents( 1000 );
        //tdc.WriteIRQVector( 3 );
        //tdc.WriteIRQLevel( 1 );

        tdc.SetReadCycles( 4 );
        tdc.AllocateBuffer();

        pA->Start();

        uint32_t nEvents = 0;
        while( nEvents < 20000 )
        {
            //controller.IRQEnable( cvIRQ1 );
            //controller.IRQWait( cvIRQ1, 2000 );
            tdc.ReadBuffer();
            UEvent<V1190B> event;
            while( tdc.GetEvent( event ) )
            {
                for( auto hit = event.cbegin(); hit != event.cend(); ++hit )
                {
                    float width = 0.4 * (hit->GetWidth());
                    hist->Fill( width );
                }
            }
            canvas->Modified();
            canvas->Update();

            nEvents += tdc.GetNEventsRead();
        }

        pA->Stop();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
