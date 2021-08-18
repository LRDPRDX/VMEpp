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

        nlohmann::json j = V895::GetDefaultConfig();

        for( uint8_t i = 0; i < desc.GetChNumber(); ++i )
        {
            j.at("settings").at("channels").at(i).at("threshold") = 10;
        }

        j.at("settings").at("majority") = 3;
        j.at("settings").at("width").at("high") = 50;
        j.at("settings").at("width").at("low") = 50;
        j.at("settings").at("mask") = 0xff;

        //j.at("settings")["imposter"] = 0xff;

        desc.WriteConfig( j );

        std::cout << j.dump( 2 ) << std::endl;
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }

    return 0;
}
