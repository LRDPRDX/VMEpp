#include "VException.h"
#include "modules/V2718.h"
#include "modules/V895.h"

#include <iostream>
#include <unistd.h>
#include <cassert>

#include "cereal/archives/xml.hpp"

using namespace vmepp;

int main()
{
    V2718 controller;
    V895 desc( 0, 0 );

    try
    {
        //controller.Open( 0, 0 );

        controller.GetPulser( cvPulserA ).SetSquare( 10, 50 );
        controller.GetPulser( cvPulserA ).SetNPulses( 88 );
        controller.GetPulser( cvPulserA ).SetStartSource( cvManualSW );
        controller.GetPulser( cvPulserA ).SetStopSource( cvManualSW );
        //controller.GetPulser( cvPulserA ).Write();

        controller.GetPulser( cvPulserB ).SetSquare( 10000, 20 );
        controller.GetPulser( cvPulserB ).SetNPulses( 200 );
        controller.GetPulser( cvPulserB ).SetStartSource( cvManualSW );
        controller.GetPulser( cvPulserB ).SetStopSource( cvManualSW );
        //controller.GetPulser( cvPulserB ).Write();

        UConfig<V2718> cfg;

        WriteConfigToFile<V2718, cereal::XMLOutputArchive>( cfg, "config.xml" );

        UConfig<V2718> cfg2;
        ReadConfigFromFile( cfg2, "config.json" );

        UConfig<V895> cfgV895;
        WriteConfigToFile( cfgV895, "config2.json" );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
