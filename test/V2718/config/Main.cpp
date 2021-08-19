#include "VException.h"
#include "modules/V2718.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cassert>

using namespace vmeplus;
using json = nlohmann::json;

int main()
{
    V2718 controller;

    try
    {
        controller.Open( 0, 0 );

        controller.GetPulser( cvPulserA ).SetSquare( 10, 50 );
        controller.GetPulser( cvPulserA ).SetNPulses( 88 );
        controller.GetPulser( cvPulserA ).SetStartSource( cvManualSW );
        controller.GetPulser( cvPulserA ).SetStopSource( cvManualSW );
        controller.GetPulser( cvPulserA ).Write();

        controller.GetPulser( cvPulserB ).SetSquare( 10000, 20 );
        controller.GetPulser( cvPulserB ).SetNPulses( 200 );
        controller.GetPulser( cvPulserB ).SetStartSource( cvManualSW );
        controller.GetPulser( cvPulserB ).SetStopSource( cvManualSW );
        controller.GetPulser( cvPulserB ).Write();

        json j;

        controller.ReadConfig( j );

        try
        {
            //WriteConfigToFile( j, "/usr/config.json" ); // causes catch
            //WriteConfigToFile( j, "./config.json" );
            j = ReadConfigFromFile( "./config.json" );
        }
        catch( std::fstream::failure& e )
        {
            std::cout << "File reading writing failed\n" << std::endl;
        }

        controller.WriteConfig( j );

        json jDefault = V2718::GetDefaultConfig();
        json jInvalid = R"({"name" : "V895"})"; 
        assert( controller.Validate( jDefault ) == true );
        assert( controller.Validate( j ) == true );
        assert( controller.Validate( jInvalid ) == false );

        //std::cout << j.dump( 2 ) << std::endl;

    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
