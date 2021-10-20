#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>
#include <vector>

using namespace vmepp;

typedef typename V1190B::MicroFirmware MF;

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

        // Micro firmware
        MF mf;
        tdc.ReadMicroFirmware( mf );
        std::cout << "REVISION: " << std::hex << mf.revision << "\n";
        std::cout << "DAY :     " << std::hex << mf.day << "\n";
        std::cout << "MONTH :   " << std::hex << mf.month << "\n";
        std::cout << "YEAR :    " << std::hex << mf.year << "\n";

        // Spare word
        tdc.WriteSpare( 0x1234 );
        uint16_t spare = tdc.ReadSpare();
        assert( spare == 0x1234 );

        // Test mode
        tdc.WriteAcqMode( V1190B::TriggerMode_t::MATCHING );
        tdc.EnableTestMode( 0x12345678 );

        tdc.DisableTestMode();

        tdc.WriteAcqMode( V1190B::TriggerMode_t::CONTINUOUS );
        tdc.EnableTestMode( 0x12345678 ); // Warning

        // Test output
        tdc.WriteTDCTestOutput( 0x1234 );

        // Clock
        tdc.WriteDLLClock( V1190B::DLLClock::DIRECT40MHz );
        tdc.WriteDLLClock( V1190B::DLLClock::PLL40MHz );
        tdc.WriteDLLClock( V1190B::DLLClock::PLL160MHz );
        tdc.WriteDLLClock( V1190B::DLLClock::PLL320MHz );

        // Write EEPROM
        std::vector<std::pair<uint16_t, uint16_t>> valid = { {0x0000, 0x0012},
                                                             {0x0001, 0x0032},
                                                             {0x0012, 0x00FF},
                                                             {0x0012, 0x00FF},
                                                             {0x0013, 0x001F},
                                                             {0x0014, 0x00AA},
                                                             {0x0015, 0x001A},
                                                             {0x0030, 0x0022}
                                                             };

        PrintMessage( Message_t::INFO, "Writing valid locations" );
        for( auto val : valid )
        {
            tdc.WriteEEPROM( val.first, val.second );
        }

        for( auto val : valid )
        {
            assert( tdc.ReadEEPROM( val.first ) == val.second );
        }

        std::vector<std::pair<uint16_t, uint16_t>> invalid = { {0x0002, 0x001F},
                                                               {0x0003, 0x001F},
                                                               {0x0004, 0x001F},
                                                               {0x0005, 0x001F},
                                                               {0x000B, 0x001F},
                                                               {0x0010, 0x001F},
                                                               {0x0022, 0x001F},
                                                               {0x0023, 0x001F}
                                                             };

        PrintMessage( Message_t::INFO, "Writing INvalid locations" );
        for( auto val : invalid )
        {
            tdc.WriteEEPROM( val.first, val.second );
        }

        for( auto val : invalid )
        {
            assert( tdc.ReadEEPROM( val.first ) != val.second );
        }
        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
