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
        //Pat pattern; 
        //tdc.ReadEnablePattern( pattern );

        //for( uint8_t i = 0; i < 4; ++i )
        //{
        //    assert( pattern.channel[i] == 0xFFFF );
        //}

        tdc.WriteEnableAll( false );
        pat0 = tdc.ReadEnablePattern( Tdc::TDC0 );
        pat1 = tdc.ReadEnablePattern( Tdc::TDC1 );
        assert( pat0 == 0x0 );
        assert( pat1 == 0x0 );
        //tdc.ReadEnablePattern( pattern );

        //for( uint8_t i = 0; i < 4; ++i )
        //{
        //    assert( pattern.channel[i] == 0x0000 );
        //}

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

        //tdc.ReadEnablePattern( pattern );

        //assert( pattern.GetChannel( 0 )  == true );
        //assert( pattern.GetChannel( 1 )  == false );
        //assert( pattern.GetChannel( 2 )  == true );
        //assert( pattern.GetChannel( 16 ) == true );
        //assert( pattern.GetChannel( 18)  == false );
        //assert( pattern.GetChannel( 19 ) == true );
        //assert( pattern.GetChannel( 33 ) == true );

        //tdc.WriteEnableChannel( 2, false );
        //tdc.ReadEnablePattern( pattern );
        //assert( pattern.GetChannel( 2 )  == false );

        //tdc.WriteEnableChannel( 66, true ); // This is actually ch 2
        //tdc.ReadEnablePattern( pattern );
        //assert( pattern.GetChannel( 2 )  == true );

        //tdc.WriteEnableChannel( 63, true );
        //tdc.WriteEnableChannel( 48, true );

        //tdc.WriteMicro( V1190B::Opcode( V1190B::Command::READ_EN_PATTERN ) );
        //uint16_t a = tdc.ReadMicro();
        //uint16_t b = tdc.ReadMicro();
        //uint16_t c = tdc.ReadMicro();
        //uint16_t d = tdc.ReadMicro();

        //std::cout << std::hex << a << " " << b << " " << c << " " << d << "\n"; 

        //tdc.WriteMicro( V1190B::Opcode( V1190B::Command::WRITE_EN_PATTERN ) );
        //tdc.WriteMicro( 0x0003 );
        //tdc.WriteMicro( 0x0030 );
        //tdc.WriteMicro( 0x0000 );
        //tdc.WriteMicro( 0x2000 );

        //tdc.WriteMicro( V1190B::Opcode( V1190B::Command::READ_EN_PATTERN ) );
        //a = tdc.ReadMicro();
        //b = tdc.ReadMicro();
        //c = tdc.ReadMicro();
        //d = tdc.ReadMicro();

        //std::cout << std::hex << a << " " << b << " " << c << " " << d << "\n"; 

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
