#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <iomanip>
#include <cassert>
#include <unistd.h>

using namespace vmeplus;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        std::cout << "Test begins...\n";
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        tdc.WriteAcqMode( V1190B::TriggerMode_t::CONTINUOUS );
        tdc.WriteDetection( V1190B::EdgeDetect_t::TRAILING );

        tdc.WriteIRQEvents( 20 );
        tdc.WriteIRQVector( 3 );
        tdc.WriteIRQLevel( 1 );

        tdc.AllocateBuffer();

        controller.IRQEnable( cvIRQ1 );

        std::cout << "Waiting for data...\n";
        controller.IRQWait( cvIRQ1, 10000 );

        uint16_t status;
        controller.IACK( cvIRQ1, &status, cvD16 );

        assert( status == 3 );

        tdc.ReadBuffer();
        V1190BEvent e;
        while( tdc.GetEvent( &e ) ) { ; }

        std::cout << "Number of events read : " << tdc.GetNEventsRead() << "\n";

        //tdc.DropBuffer( "Data.dat" );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
