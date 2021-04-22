#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::TriggerMode_t Trg_t;
typedef typename V1190B::Config_t      Cfg_t;

// This test checks that previously saved
// user configuration is indeed loaded at power on

int main()
{
    std::cout << "Test begins...\n";
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        V1190B::TriggerData td;
        tdc.ReadTriggerConfiguration( td );

        Trg_t trgMode = tdc.ReadAcqMode();

        // For asserted values see ../Main.cpp file
        assert( trgMode         == Trg_t::MATCHING );
        assert( td.winWidth     == 0x18 );
        assert( td.winOffs      == 0xFFD0 );
        assert( td.swMargin     == 0x07 );
        assert( td.rejMargin    == 0x05 );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
