#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1742B.h"

#include <iostream>
#include <unistd.h>
#include <cassert>

using namespace vmepp;

int main()
{
    V2718 controller;
    V1742B digitizer( 0x50000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &digitizer );
        controller.Initialize();

        //digitizer.WriteTRGOUTGeneration( V1742B::TrgOutSource_t::All );

        UConfig<V1742B> cfg;
        //digitizer.ReadConfig( cfg );

        //WriteConfigToFile( cfg, "config.json" );
        ReadConfigFromFile( cfg, "config.json" );
        WriteConfigToFile( cfg, "config.json" );

        digitizer.WriteConfig( cfg );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
