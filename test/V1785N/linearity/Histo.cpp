/*
   You can reproduce this test in the following way:

       1) Connect the output  
*/
#include <iostream>

#include <TH1F.h>
#include <TF1.h>

#define LINUX

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1785N.h"
#include "modules/V895.h"
#include <unistd.h>

using namespace vmepp;

void Histo()
{
    TH1I *h = new TH1I( "V1785N", "V1785N", 8192, 0, 8192 );

    V2718 controller;
    V1785N adc( 0x20000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &adc );
        controller.Initialize();

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        pA->SetSquare( 5000, 1 );
        pA->SetNPulses( 0 );//continuous pulsing
        pA->Write();//configure
        pA->Start();

        V2718::OutputConfig outCfg;
            outCfg.SOURCE = V2718::Src_t::SW;
        controller.WriteOutputConfig( V2718::Out_t::OUT0, outCfg );

        adc.EnableAll( false );
        adc.EnableZeroSupp( true );
        adc.EnableOverSupp( false );
        adc.WriteLowThreshold( 0, 1, 1 );
        adc.WriteHighThreshold( 0, 3000, 0 );
        adc.WriteIRQLevel( 7 );
        adc.WriteIRQVector( 7 );
        adc.WriteIRQEvents( 31 );

        UEvent<V1785N> event;
        adc.AllocateBuffer();

        size_t nEvents = 0;
        while( nEvents < 10000 )
        {
            controller.IRQEnable( cvIRQ7 );
            controller.IRQWait( cvIRQ7, 100 );
            adc.ReadBuffer();
            while( adc.GetEvent( event ) )
            {
                h->Fill( event.GetChannelData( 0 ) );
            }
            nEvents += adc.GetNEventsRead();
        }

        pA->Stop();

        h->Draw();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }
}
