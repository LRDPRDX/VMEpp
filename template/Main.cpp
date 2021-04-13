#include "VException.h"
#include "modules/V2718.h"
//Include boards here
//For example,
//#include "modules/V6533N.h"

#include <iostream>

using namespace vmeplus;

int main()
{
    V2718 controller;
    //Next create a board which you want to work with
    //For example,
    //V6533N hv( 0x40000000 );
    
    try
    {
        controller.Open( 0, 0 ); 
        //controller.RegisterSlave( &hv );
        //controller.Initialize();

        //********************
        //Place your code here
        //********************
        //hv.Print();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n"; 
        std::cerr << e.GetInfo() << "\n"; 
    }

    return 0;
}
