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
        controller.Open( 0, 0 ); 

        json config;

        controller.ReadConfig( config );

        std::cout << config.dump( 2 );
    }
    catch( const VException& e )
    {
        std::cerr << e.what() << "\n"; 
        std::cerr << e.GetInfo() << "\n"; 
    }
    return 0;
}
