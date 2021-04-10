#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>

using namespace vmeplus;

typedef typename V1190B::TriggerMode_t Trg_t;
typedef typename V1190B::Config_t      Cfg_t;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        /****** Trigger mode ******/
        Trg_t result = tdc.ReadAcqMode();

        tdc.WriteAcqMode( Trg_t::CONTINUOUS );
        result = tdc.ReadAcqMode();
        std::cout << "ACQ MODE : " << (int)result << "\n";

        tdc.WriteAcqMode( Trg_t::MATCHING );
        result = tdc.ReadAcqMode();
        std::cout << "ACQ MODE : " << (int)result << "\n";

        /****** Enable keep token ******/
        // Seems there is no way to read it back 
        // so just writing
        tdc.WriteEnableKeepToken( false );
        tdc.WriteEnableKeepToken( true );

        /****** Load configuration ******/
        // Load default configuration
        tdc.WriteLoadConfig( Cfg_t::DEFAULT );

        V1190B::TriggerData td; // NB : Default values for ctor are needed here
        tdc.ReadTriggerConfiguration( td );

        result = tdc.ReadAcqMode();
        std::cout << "ACQ Mode      : " << (int)result   << "\n";
        std::cout << "Window width  : " << td.WinWidth   << "\n";
        std::cout << "Window offset : " << td.WinOffs    << "\n";
        std::cout << "SW margin     : " << td.SwMargin   << "\n";
        std::cout << "Reject margin : " << td.RejMargin  << "\n";
        std::cout << "Sub Trigger   : " << td.SubTrigger << "\n";

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
        tdc.WriteWindowWidth( 0x15 );
        tdc.WriteWindowOffset( 0xFFD0 );
        tdc.WriteExtraSearchMargin( 0x07 );
        tdc.WriteRejectMargin( 0x05 );
        tdc.WriteSubstractionOfTriggerTime( 1 ); //is not actually saved

        tdc.WriteSaveUserConfig();

        // 2)
        tdc.WriteAutoLoad( Cfg_t::USER );

        // 3) Power off/on the module 

        // 4) Run the AfterReset/Main executable
        //    and sure that the previously saved
        //    config is indeed loaded at power on 
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
