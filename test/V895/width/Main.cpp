#include "VException.h"
#include "modules/V2718.h"
#include "modules/V895.h"

#include <iostream>
#include <unistd.h>

using namespace vmepp;

int main()
{
    V2718 controller;
    //Instantiate a descriminator
    V895 dm( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &dm );
        controller.Initialize();

        //Print some info
        dm.Print();

        dm.WriteOutWidth( 0 );
        std::cout << "W(0) = " << dm.GetValueFromWidth(0) << "\n";
        std::cout << "W(4) = " << dm.GetValueFromWidth(4) << "\n";
        std::cout << "W(5) = " << dm.GetValueFromWidth(5) << "\n";
        std::cout << "W(7) = " << dm.GetValueFromWidth(7) << "\n";
        std::cout << "W(20) = " << dm.GetValueFromWidth(20) << "\n";
        std::cout << "W(30) = " << dm.GetValueFromWidth(30) << "\n";
        std::cout << "W(40) = " << dm.GetValueFromWidth(40) << "\n";
        std::cout << "W(42) = " << dm.GetValueFromWidth(42) << "\n";
        std::cout << "W(43) = " << dm.GetValueFromWidth(43) << "\n";
        std::cout << "W(44) = " << dm.GetValueFromWidth(44) << "\n";
        std::cout << "W(48) = " << dm.GetValueFromWidth(48) << "\n";
        std::cout << "W(100) = " << dm.GetValueFromWidth(100) << "\n";
    }
    catch( const VException& e )
    {
        std::cerr << e.what() << "\n";
        std::cerr << e.GetInfo() << "\n";
    }
    return 0;
}
