#include <iostream>

#include <TH1F.h>
#include <TF1.h>

#define LINUX

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1785N.h"
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

        adc.EnableAll( false );
        adc.WriteLowThreshold( 0, 0, 0 );
        adc.WriteHighThreshold( 0, 0, 0 );
        adc.WriteIRQLevel( 7 );
        adc.WriteIRQVector( 7 );
        adc.WriteIRQEvents( 31 );

        UEvent<V1785N> event;
        adc.AllocateBuffer();

        controller.GetPulser( cvPulserA ).SetSquare( 100, 1 );//100 kHz, 1 % duty cycle
        controller.GetPulser( cvPulserA ).SetNPulses( 0 );//continuous pulsing
        controller.GetPulser( cvPulserA ).Write();//configure

        controller.WriteOutputConfig( cvOutput0, cvMiscSignals );

        controller.GetPulser( cvPulserA ).Start();

        for( int i = 0; i < 10; i++ )
        {
            controller.IRQEnable( cvIRQ7 );
            controller.IRQWait( cvIRQ7, 100 );
            adc.ReadBuffer();
            while( adc.GetEvent( event ) )
            {
                h->Fill( event.GetChannelData( 0 ) );
            }
            std::cout << "Events read : " << adc.GetNEventsRead() << "\n";
        }

        controller.GetPulser( cvPulserA ).Stop();

        h->Draw();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }
}
