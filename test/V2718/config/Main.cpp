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
    
        controller.Open( 0, 0 ); 

        json j;

        std::cout << controller.GetDefaultConfig().dump( 2 ) << "\n";

        controller.ReadConfig( j );
    return 0;
}
