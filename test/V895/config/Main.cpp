#include "VException.h"

#include "modules/V895.h"
#include "modules/V2718.h"

#include <iostream>

using namespace vmeplus;

int main()
{
    V2718 controller;
    //Next create a board which you want to work with
    //For example,
    V895 desc( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &desc );//Connect the board we want to work with
        controller.Initialize();//Initialize all the registered boards

        UConfig<V895> cfg;
        desc.ReadConfig( cfg );

        WriteConfigToFile( cfg, "config.json" );
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }

    return 0;
}
