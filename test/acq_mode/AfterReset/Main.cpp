#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>

using namespace vmeplus;

typedef typename V1190B::TriggerMode_t Trg_t;
typedef typename V1190B::Config_t      Cfg_t;

// This test checks that previously saved
// user configuration is indeed loaded at power on

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        V1190B::TriggerData td;
        tdc.ReadTriggerConfiguration( td );

        Trg_t result = tdc.ReadAcqMode();
        std::cout << "ACQ Mode      : " << (int)result   << "\n";
        std::cout << "Window width  : " << td.WinWidth   << "\n";
        std::cout << "Window offset : " << td.WinOffs    << "\n";
        std::cout << "SW margin     : " << td.SwMargin   << "\n";
        std::cout << "Reject margin : " << td.RejMargin  << "\n";
        std::cout << "Sub Trigger   : " << td.SubTrigger << "\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
