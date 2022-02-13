#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1785N.h"

#include <iostream>
#include <unistd.h>

using namespace vmepp;

int main()
{
    V2718 controller;
    V1785N adc( 0x20000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &adc );
        controller.Initialize();

        adc.WriteIRQEvents( 5 );
        adc.WriteIRQLevel( 7 );
        adc.WriteIRQVector( 25 );

        std::cout << adc.ReadIRQEvents() << "\n";

        adc.ClearData();

        controller.IRQEnable( cvIRQ7 );
        controller.IRQWait( cvIRQ7, 10000 );
        uint16_t vector;
        controller.IACK( cvIRQ7, &vector, cvD16 );
        std::cout << "STATUS : " << vector << "\n";

        VBuffer data;
        adc.ReadBuffer( data );

        UParser parser;
        UEvent<V1785N> event;
        while( parser.GetEvent( event, data ) ) { ; }

        std::cout << "Number of channels active : " << event.GetMemoChannels() << "\n";
        std::cout << "Events read : " << parser.GetNEventsRead() << "\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }

    return 0;
}
