#include "VException.h"
#include "modules/V2718.h"  // controller
#include "modules/V895.h"   // discriminator
#include "modules/V1190B.h" // TDC

#include <iostream>

int main()
{
    V2718 controller;
    V895 dm( 0x40080000 );
    V1190B tdc( 0x20080000 );

    try
    {
        controller.RegisterSlave( &dm );
        controller.RegisterSlave( &tdc );

        controller.Initialize();
    }
    catch( const VException& e )
    {
        std::cout << e.what() << std::endl;
        std::cout << e.GetInfo() << std::endl;
    }
}
