#include "VSlave.h"
#include "VException.h"
#include "VMaster.h"

#include <iomanip>


namespace vmeplus
{
    VSlave::VSlave( std::string name, uint32_t baseAddress, uint32_t range ) :
        fMaster( nullptr ),
        fBaseAddress( baseAddress ),
        fRange( range ),

        fName( name ),
        fFirmware( "N/A" ),
        fSerial( 0 )
    {};

    VSlave::~VSlave()
    {
        Release();
    };

    void VSlave::ReadRequest( uint32_t address, void *data, CVDataWidth dw, CVAddressModifier am )
    {
        if( fMaster != nullptr )
        {
            fMaster->ReadCycle( fBaseAddress + address, data, am, dw );
        }
        else
        {
            throw VException( VError_t::vOrphan, "Try to register the slave to a controller" );
        }
    }

    void VSlave::WriteRequest( uint32_t address, void *data, CVDataWidth dw, CVAddressModifier am )
    {
        if( fMaster != nullptr )
        {
            fMaster->WriteCycle( fBaseAddress + address, data, am, dw );
        }
        else
        {
            throw VException( VError_t::vOrphan, "Try to register the slave to a controller" );
        }
    }

    void VSlave::BLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVDataWidth dw, CVAddressModifier am )
    {
        if( fMaster != nullptr )
        {
            fMaster->BLTReadCycle( fBaseAddress + address, buffer, size, am, dw, count );
        }
        else
        {
            throw VException( VError_t::vOrphan, "Try to register the slave to a controller" );
        }
    }

    void VSlave::MBLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVAddressModifier am )
    {
        if( fMaster != nullptr )
        {
            fMaster->MBLTReadCycle( fBaseAddress + address, buffer, size, am, count );
        }
        else
        {
            throw VException( VError_t::vOrphan, "Try to register the slave to a controller" );
        }
    }

    void VSlave::FIFOMBLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVAddressModifier am )
    {
        if( fMaster != nullptr )
        {
            fMaster->FIFOMBLTReadCycle( fBaseAddress + address, buffer, size, am, count );
        }
        else
        {
            throw VException( VError_t::vOrphan, "Try to register the slave to a controller" );
        }
    }

    uint16_t VSlave::ReadRegister16( uint32_t address, uint16_t msk )
    {
        uint16_t data;
        ReadRequest( address, &data );//D16 by default
        return (data & msk);
    }

    void VSlave::WriteRegister16( uint32_t address, uint16_t data, uint16_t msk )
    {
        data &= msk;
        WriteRequest( address, &data );//D16 by default
    }

    uint32_t VSlave::ReadRegister32( uint32_t address, uint32_t msk ){
        uint32_t data;
        ReadRequest( address, &data, cvD32 );
        return (data & msk);
    }

    void VSlave::WriteRegister32( uint32_t address, uint32_t data, uint32_t msk ) {
        data &= msk;
        WriteRequest( address, &data, cvD32 );
    }

    void VSlave::SetBit16( uint32_t address, uint16_t bit )
    {
        uint16_t data = ReadRegister16( address );
        data |= (1U << bit);
        WriteRegister16( address, data );
    }

    void VSlave::ClearBit16( uint32_t address, uint16_t bit )
    {
        uint16_t data = ReadRegister16( address );
        data &= ~(1U << bit);
        WriteRegister16( address, data );
    }

    bool VSlave::GetBit16( uint32_t address, uint16_t bit )
    {
        return ReadRegister16( address ) & (1U << bit);
    }

    //
    void VSlave::SetBaseAddress( uint32_t newAddress )
    {
        if( fMaster == nullptr )
        {
            fBaseAddress = newAddress;
        }
        else
        {
            PrintMessage( Message_t::WARNING, "Trying to change the base address *AFTER* registration" );
        }
    }

    void VSlave::Reset()
    {
        PrintMessage( Message_t::INFO, "Resetting " + fName + "...\n" ); 
        PrintMessage( Message_t::INFO, "Resetting " + fName + "...OK\n" );
    }

    void VSlave::Print() const
    {
        std::cout << "\n";
        std::cout << std::right << "     /" << std::setfill('=') << std::setw(60) << "=" <<                                                                        "/-\n";
        std::cout << std::right << "    /" << std::setfill(' ') << std::setw(60) << " " <<                                                                        "/-|\n";
        std::cout << std::right << "   |" << std::setfill('=') << std::setw(60) << "=" <<                                                                        "/- |\n";
        std::cout << std::setfill(' ');
        std::cout << std::right << "   |" << std::setw(30) << fName << std::setw(30) << " " <<                                                                  "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                           "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "| /-\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|/-\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                           "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                           "|-_/\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|-/ \n";
        std::cout << std::right << "   |" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "|/\n";
        std::cout << std::setfill(' ');
        std::cout << "\n";
    }

    void VSlave::Release()
    {
        if( fMaster != nullptr )
        {
            fMaster->UnregisterSlave( this );
        }
    }
}
