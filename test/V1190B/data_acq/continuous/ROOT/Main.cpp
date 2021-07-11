#include <iostream>

#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>

#define LINUX

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

using namespace vmeplus;

void Histo()
{
    TCanvas* c = new TCanvas( "c", "c", 800, 800 );
    TH1F *h = new TH1F( "V1190B", "V1190B", 100, 0, 100 );
        h->Draw();

    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        std::cout << "Test begins...\n";

        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        controller.WriteOutputConfig( static_cast<CVOutputSelect>( 0 ), cvMiscSignals );
        controller.GetPulser( cvPulserA ).SetSquare( 1000, 50 );
        controller.GetPulser( cvPulserA ).SetNPulses( 0 );
        controller.GetPulser( cvPulserA ).SetStartSource( cvManualSW );
        controller.GetPulser( cvPulserA ).SetStopSource( cvManualSW );
        controller.GetPulser( cvPulserA ).Write();

        controller.GetPulser( cvPulserA ).Start();

        tdc.WriteAcqMode( V1190B::TriggerMode_t::CONTINUOUS );
        tdc.WriteDetection( V1190B::EdgeDetect_t::PAIR );
        V1190B::PairRes pRes = V1190B::PairRes( V1190B::ResLeadEdgeTime::ps100, V1190B::ResPulseWidth::ps800 );
        tdc.WritePairRes( pRes );

        tdc.AllocateBuffer();

        uint32_t nEvents = 0;
        while( nEvents < 20000 )
        {
            tdc.ReadBuffer();
            V1190BEvent e;
            while( tdc.GetEvent( &e ) )
            {
                for( auto it = e.cbegin(); it != e.cend(); ++it )
                {
                    h->Fill( 0.8 * (int)(it->GetWidth()) );
                }
            }
            c->Modified();
            c->Update();

            nEvents += tdc.GetNEventsRead(); 
        }

        std::cout << "Mean : " << h->GetMean() << "\n";

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
