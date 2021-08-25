#include "VException.h"
#include "modules/V2718.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cassert>

using namespace vmeplus;

int main()
{
    V2718 controller;

    try
    {
        for( uint32_t freq = 1; freq < 50; ++freq )
        {
            V2718Pulser& pulser = controller.GetPulser( cvPulserA );
            pulser.SetSquare( freq, 50 );
            std::cout << freq << "\t" << pulser.GetFrequencyReal() << "\n";
        }
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
