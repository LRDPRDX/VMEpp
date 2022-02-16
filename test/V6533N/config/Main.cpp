#include "VException.h"

#include "modules/V6533N.h"
#include "modules/V2718.h"

#include <iostream>

#include "cereal/archives/xml.hpp"

using namespace vmepp;

int main()
{
    V2718 controller;
    V6533N hv( 0x40000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &hv );//Connect the board we want to work with
        controller.Initialize();//Initialize all the registered boards

        UConfig<V6533N> cfg;

        hv.ReadConfig( cfg );//Read config from module
        hv.WriteConfig( cfg );//Write config to module

        WriteConfigToFile<V6533N, cereal::XMLOutputArchive>( cfg, "/usr/config.xml" );//Write config to file
    }
    catch( const VException &cv )
    {
        std::cerr << cv.what() << "\n";
        std::cerr << cv.GetInfo() << "\n";
    }

    return 0;
}
