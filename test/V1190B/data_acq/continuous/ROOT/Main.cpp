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
    TCanvas* c = new TCanvas( "c", "c", 800, 800 );
    TH1F *h = new TH1F( "V1190B", "V1190B", 100, 0, 100 );
        h->Draw();

    V2718 controller;
    V1190B tdc( 0x20080000 );
    V895 disc( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &tdc );
        controller.RegisterSlave( &disc );
        controller.Initialize();

        disc.EnableOnly( 14 );
        disc.WriteOutWidth( 30 );

        V2718::OutputConfig outCfg;
        outCfg.SOURCE = V2718::Src_t::PULS_SCAL;
        controller.WriteOutputConfig( V2718::Out_t::OUT0, outCfg );

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        pA->SetSquare( 5000, 1 );
        pA->SetNPulses( 0 );
        pA->SetStartSource( V2718::Src_t::SW );
        pA->SetStopSource( V2718::Src_t::SW );
        pA->Write();
        pA->Start();

        tdc.WriteDetection( V1190B::EdgeDetect_t::PAIR );
        V1190B::PairRes pRes;
            pRes.EDGE = V1190B::ResLeadEdgeTime::ps100;
            pRes.WIDTH = V1190B::ResPulseWidth::ps400;
        tdc.WritePairRes( pRes );
        tdc.WriteWindowWidth( 20 ); // 500 ns
        tdc.WriteWindowOffset( -10 ); // -250 ns
        tdc.WriteEnableSubTrigger( true );
        tdc.WriteControl( V1190B::Control_t::EVENT_FIFO_EN, false );

        tdc.WriteAcqMode( V1190B::TriggerMode_t::CONTINUOUS );

        tdc.WriteIRQEvents( 2048 );
        tdc.WriteIRQVector( 3 );
        tdc.WriteIRQLevel( 1 );

        tdc.AllocateBuffer();

        uint32_t nEvents = 0;
        while( nEvents < 20000 )
        {
            controller.IRQEnable( cvIRQ1 );
            controller.IRQWait( cvIRQ1, 2048 );
            tdc.ReadBuffer();
            UEvent<V1190B> event;
            while( tdc.GetEvent( event ) )
            {
                for( auto it = event.cbegin(); it != event.cend(); ++it )
                {
                    std::cout << it->value << "\n";
                    float width = 0.4 * (it->GetWidth());
                    if( width > 0.0 )
                    {
                        h->Fill( width );
                    }
                }
            }
            c->Modified();
            c->Update();

            nEvents += tdc.GetNEventsRead();
            //tdc.WriteSoftwareClear();
        }

        pA->Stop();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
