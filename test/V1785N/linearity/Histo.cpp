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
    V895 disc( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &adc );
        controller.RegisterSlave( &disc );
        controller.Initialize();

        disc.WriteOutWidthH( 40 );
        disc.EnableOnly( 15 );

        controller.WriteOutputConfig( cvOutput0, cvMiscSignals );
        controller.GetPulser( cvPulserA ).SetSquare( 1000, 1 );
        controller.GetPulser( cvPulserA ).SetNPulses( 0 );//continuous pulsing
        controller.GetPulser( cvPulserA ).Write();//configure
        controller.GetPulser( cvPulserA ).Start();

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

        controller.GetPulser( cvPulserA ).Stop();

        h->Draw();
        std::cout << h->GetMean() << " " << h->GetStdDev() << "\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n"; 
        std::cerr << e.GetInfo() << "\n";
    }
}
