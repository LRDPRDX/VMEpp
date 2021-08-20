#include "VException.h"

#include "modules/V6533N.h"
#include "modules/V2718.h"

#include <iostream>

using namespace vmeplus;

int main()
{
    V2718 controller;
    //Next create a board which you want to work with
    //For example,
    V6533N hv( 0x40000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &hv );//Connect the board we want to work with
        controller.Initialize();//Initialize all the registered boards

        UConfig<V6533N> cfg;

        hv.ReadConfig( cfg );
        hv.WriteConfig( cfg );
    }
    catch( const VException &cv )
    {
        std::cerr << cv.what() << "\n";
        std::cerr << cv.GetInfo() << "\n";
    }

    return 0;
}
