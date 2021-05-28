#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace vmeplus;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        std::cout << "Test begins...\n";
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        std::cout << std::setw( 30 ) << "NAME : " << tdc.GetName() << "\n"; 
        std::cout << std::setw( 30 ) << "BASE ADDRESS : " << std::hex << tdc.GetBaseAddress() << "\n"; 
        std::cout << std::setw( 30 ) << "RANGE : " << std::hex << tdc.GetRange() << "\n"; 
        std::cout << std::setw( 30 ) << "OUI : " << tdc.GetOUI() << "\n"; 
        std::cout << std::dec;
        std::cout << std::setw( 30 ) << "FIRMWARE : " << tdc.GetFirmware() << "\n"; 
        std::cout << std::setw( 30 ) << "BOARD ID : " << tdc.GetBoardID() << "\n"; 
        std::cout << std::setw( 30 ) << "VERSION : " << tdc.GetVersion() << "\n"; 
        std::cout << std::setw( 30 ) << "REVISION : " << tdc.GetRevision() << "\n"; 
        std::cout << std::setw( 30 ) << "SERIAL NUMBER : " << tdc.GetSerialNumber() << "\n"; 

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
