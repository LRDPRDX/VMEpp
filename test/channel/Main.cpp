#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::ChannelPattern Pat;

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

        tdc.WriteEnableAll( true );

        Pat pattern; 
        pattern = tdc.ReadEnablePattern();

        for( uint8_t i = 0; i < 4; ++i )
        {
            assert( pattern.channel[i] == 0xFFFF );
        }

        tdc.WriteEnableAll( false );
        pattern = tdc.ReadEnablePattern();

        for( uint8_t i = 0; i < 4; ++i )
        {
            // By default all channels are enabled
            assert( pattern.channel[i] == 0x0000 );
        }

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
