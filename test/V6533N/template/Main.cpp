#include "VException.h"

#include "modules/V6533N.h"
#include "modules/V2718.h"

#include <unistd.h>
#include <iostream>

using namespace vmeplus;

int main()
{
    V2718 controller;
    //Next create a board which you want to work with
    //For example,
    V6533N hv( 0x40000000 );
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &hv );//Connect the board we want to work with
        controller.Initialize();//Initialize all the registered boards

        uint16_t nChannels = hv.ReadNChannels();//Get number of available channels
        //for( uint16_t ch = 0; ch < nChannels; ch++ )
        //{
        //    hv.WriteVoltage( ch, 50 );//50 V on channel <ch>
        //    hv.WriteEnable( ch, true );//Turn ON channel <ch>

        //    usleep( 500000 );//wait for a half second

        //    hv.WriteEnable( ch, false );//Turn OFF channel <ch>
        //}
        std::cout << hv.ReadVMax() << "\n";
        std::cout << hv.ReadIMax() << "\n";
        
        hv.WriteRampUp( 0, 400 );
        hv.WriteRampDown( 0, 400 );
        hv.WriteVoltage( 0, 1225 );
        hv.WriteEnable( 0, true );
        hv.WriteEnable( 0, false );
        //hv.Print();
    }
    catch( VException<CVErrorCodes> &cv )
    {
        std::cerr << cv.what() << "\n"; 
        std::cerr << cv.GetInfo() << "\n"; 
    }
    catch( VException<SWErrorCodes> &sw )
    {
        std::cerr << sw.what() << "\n"; 
        std::cerr << sw.GetInfo() << "\n"; 
    }

    return 0;
}
