#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1785N.h"

#include <iostream>
#include <unistd.h>
#include <cassert>

#include "cereal/archives/xml.hpp"

using namespace vmepp;

int main()
{
    V2718 controller;
    V1785N adc( 0x20000000 );

    try
    {
        controller.Open( 0, 0 );

        controller.RegisterSlave( &adc );

        adc.EnableOverSupp( false );
        adc.EnableZeroSupp( false );
        adc.WriteIRQVector( 3 );

        UConfig<V1785N> cfg;
        adc.ReadConfig( cfg );

        WriteConfigToFile( cfg, "config.json" );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
