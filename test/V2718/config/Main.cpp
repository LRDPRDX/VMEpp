#include "VException.h"
#include "modules/V2718.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <unistd.h>

using namespace vmeplus;
using json = nlohmann::json;

int main()
{
    V2718 controller;

    try
    {
        //controller.Open( 0, 0 );

        json j;

        std::cout << controller.GetDefaultConfig().dump( 2 ) << "\n";

        controller.ReadConfig( j );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
    }
    return 0;
}
