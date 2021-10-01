/*
   To test this example you may use the following configuration:
   
   1) Connect any output (say ch 12) of the Descriminator V895 to the
      TRIGGER input of the CONTROL connector of the TDC V1190B

   2) Connect the output (ch 12) of the Descriminator V895 to the input of TDC V1190B

   3) Send the Test signal to the V895 N times (see "WriteIRQEvents" parameter )
*/
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
        tdc.WriteDetection( V1190B::EdgeDetect_t::PAIR );
        V1190B::PairRes pRes = V1190B::PairRes( V1190B::ResLeadEdgeTime::ps100, V1190B::ResPulseWidth::ps800 );
        tdc.WritePairRes( pRes );

        tdc.WriteIRQEvents( 8 );
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
        UEvent<V1190B> e;
        while( tdc.GetEvent( e ) )
        {
            for( auto it = e.cbegin(); it != e.cend(); ++it )
            {
                std::cout << "Width : " << 0.8 * (int)(it->GetWidth()) << "\n";
            }
        }

        std::cout << "Number of events read : " << tdc.GetNEventsRead() << "\n";

        tdc.DropBuffer( "/usr/tmp/dum.dat" );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
