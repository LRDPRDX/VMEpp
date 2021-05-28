#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::TriggerMode_t Trg_t;
typedef typename V1190B::Config_t      Cfg_t;

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

        /****** Trigger mode ******/
        tdc.WriteAcqMode( Trg_t::CONTINUOUS );
        Trg_t trgMode = tdc.ReadAcqMode();
        assert( trgMode == Trg_t::CONTINUOUS );

        tdc.WriteAcqMode( Trg_t::MATCHING );
        trgMode = tdc.ReadAcqMode();
        assert( trgMode == Trg_t::MATCHING );

        /****** Enable keep token ******/
        // Seems there is no way to read it back 
        // so just writing
        tdc.WriteEnableKeepToken( false );
        tdc.WriteEnableKeepToken( true );

        /****** Load configuration ******/
        // Load default configuration
        tdc.WriteLoadConfig( Cfg_t::DEFAULT );
        // NB: Subtraction trigger time doesn't load by previous call
        // That's why we set it manually
        tdc.WriteEnableSubTrigger( true );

        V1190B::TriggerData tdDef; // Initialized with default values
        V1190B::TriggerData tdRead; // NB : Default values for ctor are needed here
        tdc.ReadTriggerConfiguration( tdRead );

        trgMode = tdc.ReadAcqMode();
        assert( trgMode             == Trg_t::CONTINUOUS );
        assert( tdRead.winWidth     == tdDef.winWidth );
        assert( tdRead.winOffs      == tdDef.winOffs );
        assert( tdRead.swMargin     == tdDef.swMargin );
        assert( tdRead.rejMargin    == tdDef.rejMargin );
        assert( tdRead.subTrigger   == true );

        /****** Set auto load ******/
        // To test this feature we should act
        // in the following way :
        // 1) Save the current configuration
        // 2) Make the User Config (saved previously)
        //    to load at power on
        // 3) Reset the TDC (Power on and off, or Global reset)
        // 4) Read the config after reset

        // 1)
        tdc.WriteAcqMode( Trg_t::MATCHING );
        tdc.WriteWindowWidth( 0x18 );
        tdc.WriteWindowOffset( 0xFFD0 );
        tdc.WriteExtraSearchMargin( 0x07 );
        tdc.WriteRejectMargin( 0x05 );
        tdc.WriteEnableSubTrigger( false ); //is not actually saved

        tdc.WriteSaveUserConfig();

        tdc.WriteLoadConfig( Cfg_t::USER );
        tdc.ReadTriggerConfiguration( tdRead );

        trgMode = tdc.ReadAcqMode();
        assert( trgMode             == Trg_t::MATCHING );
        assert( tdRead.winWidth     == 0x18 );
        assert( tdRead.winOffs      == 0xFFD0 );
        assert( tdRead.swMargin     == 0x07 );
        assert( tdRead.rejMargin    == 0x05 );
        assert( tdRead.subTrigger   == false );

        // 2)
        tdc.WriteAutoLoad( Cfg_t::USER );

        // 3) Power off/on the module 

        // 4) Run the AfterReset/Main executable
        //    and sure that the previously saved
        //    config is indeed loaded at power on 
        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
