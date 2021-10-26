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

    try
    {
        controller.Open( 0, 0 );

        V2718::OutputConfig oCfg;
            oCfg.LED_POLARITY = V2718::LEDPolarity_t::ACTIVE_LOW;

        //controller.WriteOutputConfig( V2718::Out_t::OUT0, oCfg );

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        V2718::Pulser* pB = controller.GetPulser( V2718::Pulser_t::B );

        pA->SetSquare( 11, 50 );
        pA->SetNPulses( 88 );
        pA->SetStartSource( V2718::Src_t::SW );
        pA->SetStopSource( V2718::Src_t::SW );
        pA->Write();

        pB->SetSquare( 10000, 23 );
        pB->SetNPulses( 240 );
        pB->SetStartSource( V2718::Src_t::SW );
        pB->SetStopSource( V2718::Src_t::SW );
        pB->Write();

        UConfig<V2718> cfg;

        controller.ReadConfig( cfg );
        WriteConfigToFile<V2718, cereal::XMLOutputArchive>( cfg, "config.xml" );

        //UConfig<V2718> cfg2;
        //ReadConfigFromFile( cfg2, "config.json" );

        //UConfig<V895> cfgV895;
        //WriteConfigToFile( cfgV895, "config2.json" );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
