#include "VException.h"
#include "modules/V2718.h"

#include <iostream>
#include <fstream>
#include <fstream>
#include <unistd.h>
#include <cassert>

using namespace vmeplus;

int main()
{
    V2718 controller;

    try
    {
        unsigned fails = 0;
        std::ofstream file( "./data.dat" );
        for( uint32_t freq = 1; freq < 10000000; ++freq )
        {
            V2718Pulser& pulser = controller.GetPulser( cvPulserA );
            if( pulser.SetSquare( freq, 50 ) )
            {
                double error = ((double)freq - pulser.GetFrequencyReal()) / (double)freq;
                file << freq << "\t" << error << "\n";
            }
            else
            {
                std::cout << freq << "\n";
                fails++;
            }
        }

        std::cout << "Fails : " << fails << "\n";

        file.close();
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
