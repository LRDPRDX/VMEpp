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

        V2718::OutputConfig outCfg;
            outCfg.SOURCE = V2718::Src_t::PULS_SCAL;
        controller.WriteOutputConfig( V2718::Out_t::OUT0, outCfg );

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        // Start and stop from software
        pA->SetStartSource( V2718::Src_t::SW );
        pA->SetStopSource( V2718::Src_t::SW );
        // 5000 Hz, 1% duty cycle
        pA->SetSquare( 5000, 1 );
        // Continuous pulsing
        pA->SetNPulses( 0 );
        // Configure pulser
        pA->Write();

        // Disable all channels
        adc.EnableAll( false );
        // Don't save under threshold
        adc.EnableZeroSupp( true );
        // Save over threshold
        adc.EnableOverSupp( false );
        // Low Range Disable
        adc.WriteLowThreshold( 0, 1, 1 );
        // High Range Enable
        adc.WriteHighThreshold( 0, 0, 0 );
        // Interrupt settings
        adc.WriteIRQLevel( 7 );
        adc.WriteIRQVector( 7 );
        adc.WriteIRQEvents( 31 );

        UEvent<V1785N> event;
        adc.AllocateBuffer();

        // Start pulser
        pA->Start();

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
