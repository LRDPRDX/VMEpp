#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1742B.h"

#include <iostream>
#include <unistd.h>
#include <cassert>

#include "cereal/archives/xml.hpp"

using namespace vmepp;

int main()
{
    V2718 controller;
    V1742B digitizer( 0x50000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &digitizer );
        controller.Initialize();

        digitizer.WriteReadoutEnableGroup( V1742B::Group_t::G0, true );
        digitizer.WriteReadoutEnableTR( true );
        digitizer.WriteRecordLength( V1742B::RecordLength_t::s1024 );
        digitizer.WriteSamplingRate( V1742B::SamplingRate_t::M5000 );
        digitizer.WriteAcqMode( V1742B::AcqMode_t::Output );

        digitizer.Start();
        usleep( 50000 );
        digitizer.WriteSWTrigger();
        usleep( 50000 );
        digitizer.WriteSWTrigger();
        digitizer.Stop();

        VBuffer buffer;
        digitizer.ReadBuffer( buffer );
        std::cout << "Buffer size: " << buffer.GetSize() << "\n";

        UParser parser;
        UEvent<V1742B> event;
        while( parser.GetEvent( event, buffer ) )
        {
            event.Print();
            auto g = event.GetGroup( V1742B::Group_t::G0 );
            auto tr = g.GetTR();
            std::cout << tr.size() << "\n";
            g.Print();
        }

        buffer.Dump( "data.dat" );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
