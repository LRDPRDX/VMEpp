#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

//typedef typename V1190B::ChannelPattern Pat;
typedef typename V1190B::TDC Tdc;

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

        uint32_t pat0 = tdc.ReadEnablePattern( Tdc::TDC0 );
        uint32_t pat1 = tdc.ReadEnablePattern( Tdc::TDC1 );

        assert( pat0 == 0xffffffff );
        assert( pat1 == 0xffffffff );

        tdc.WriteEnableAll( false );
        pat0 = tdc.ReadEnablePattern( Tdc::TDC0 );
        pat1 = tdc.ReadEnablePattern( Tdc::TDC1 );
        assert( pat0 == 0x0 );
        assert( pat1 == 0x0 );

        tdc.WriteEnableChannel( 0 );
        tdc.WriteEnableChannel( 2 );
        tdc.WriteEnableChannel( 16 );
        tdc.WriteEnableChannel( 19 );

        tdc.WriteEnableChannel( 34 );
        tdc.WriteEnableChannel( 43 );

        pat0 = tdc.ReadEnablePattern( Tdc::TDC0 );
        pat1 = tdc.ReadEnablePattern( Tdc::TDC1 );

        assert( pat0 == ((1 << 0) | (1 << 2) | (1 << 16) | (1 << 19)) );
        assert( pat1 == ((1 << 2) | (1 << 11)) );

        tdc.WriteEnablePattern( Tdc::TDC0, 0xffff0000 );
        tdc.WriteEnablePattern( Tdc::TDC1, 0x0000ffff );

        pat0 = tdc.ReadEnablePattern( Tdc::TDC0 );
        pat1 = tdc.ReadEnablePattern( Tdc::TDC1 );

        assert( pat0 == 0xffff0000 );
        assert( pat1 == 0x0000ffff );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
