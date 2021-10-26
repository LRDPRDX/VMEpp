#include "modules/V895.h"
#include "VException.h"

#include <iostream>
#include <cmath>
#include <iomanip>

namespace vmepp
{
    V895::V895( uint32_t baseAddress, uint32_t range ) :
        VSlave( baseAddress, range ),

        fVersionSerial( 0 ),
        fManMType( 0 ),
        fFixedCode( 0 )
    {
    }

    template<>
    const std::string UConfigurable<V895>::fName = "V895";

    void V895::Initialize()
    {
        PrintMessage( Message_t::INFO, "Initializing " + fName + "..." );

        ReadVerSer();
        ReadManMType();
        ReadFixedCode();

        for( uint8_t ch = 0; ch < fChNumber; ch++ )
        {
            WriteThreshold( ch, 0xFF );
        }

        PrintMessage( Message_t::INFO, "Initializing " + fName + "...OK" );

        return;
    }


    //*********************
    //Channel configuration
    //*********************
    void V895::WriteThreshold( uint8_t ch, uint8_t thr )
    {
        WriteRegister16( V895_THRESHOLD(ch % fChNumber), thr, V895_THRESHOLD_MSK );
    }

    //***************************
    //Board general configuration
    //***************************
    uint16_t V895::GetValueFromWidth( uint16_t width )
    {
        uint16_t w = (width < 5) ? 5 : ((width < 43) ? width : 43);
        return std::round( 284. * (1. - (4.3 / w)) );
    }

    void V895::WriteOutWidth( uint16_t width )
    {
        WriteOutWidthL( width );
        WriteOutWidthH( width );
    }

    void V895::WriteOutWidthH( uint16_t width )
    {
        WriteRegister16( V895_OUT_WIDTH_H, GetValueFromWidth( width ), V895_OUT_WIDTH_MSK );
    }

    void V895::WriteOutWidthL( uint16_t width )
    {
        WriteRegister16( V895_OUT_WIDTH_L, GetValueFromWidth( width ), V895_OUT_WIDTH_MSK );
    }

    void V895::WriteMajLevel( uint16_t level )
    {
        uint16_t goodLevel = (level >= 1U) ? ((level <= fChNumber) ? level : fChNumber) : 1U;
        uint16_t majThr = lround( (goodLevel * 50. - 25.) / 4 );//See the module's manual
        WriteRegister16( V895_MAJ_THRESHOLD, majThr, V895_MAJ_THR_MSK );
    }

    void V895::Enable( uint16_t mask )
    {
        WriteRegister16( V895_PAT_INHIBIT, mask );
    }

    void V895::EnableOnly( uint8_t ch )
    {
        uint16_t mask = (1 << (ch % fChNumber));
        WriteRegister16( V895_PAT_INHIBIT, mask );
    }

    void V895::SendTest()
    {
        uint16_t dummy = 1;
        WriteRegister16( V895_TEST_PULSE, dummy );
    }

    //*****************
    //Board information
    //*****************
    uint16_t V895::ReadVerSer()
    {
        fVersionSerial = ReadRegister16( V895_VER_SER );
        fSerial = fVersionSerial & V895_SERIAL_MSK;
        return fVersionSerial;
    }

    uint16_t V895::ReadManMType()
    {
        fManMType = ReadRegister16( V895_MAN_MTYPE );
        return fManMType;
    }

    uint16_t V895::ReadFixedCode()
    {
        fFixedCode = ReadRegister16( V895_FIXED_CODE );
        return fFixedCode;
    }

    //******************
    //Additional methods
    //******************
    void V895::Print() const
    {
        std::cout << "\n";
        std::cout << std::right << "     /" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "/-\n";
        std::cout << std::right << "    /" << std::setfill(' ') << std::setw(60) << " " <<                                                                       "/-|\n";
        std::cout << std::right << "   /" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "/- |\n";
        std::cout << std::setfill(' ');
        std::cout << std::right << "   |" << std::setw(30) << fName << std::setw(30) << " " <<                                                                  "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << " /\"|" << std::setw(60) << " " <<                                                                                           "|- |\n";
        std::cout << std::right << "/__|" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "\\__|" << std::setw(60) << " " <<                                                                                           "|- _\n";
        std::cout << std::right << "  ||" << std::setw(60) << " " <<                                                                                            "|-_\n";
        std::cout << std::right << "  ||" << std::setw(30) << "Serial Number : " << std::left << std::setw(30) << GetSerialNumber() <<                          "|- |\n";
        std::cout << std::right << " /||" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "/_||" << std::setw(30) << "Version : " << std::left << std::setw(30) << GetVersion() <<                                     "|  |\n";
        std::cout << std::right << "\\__|" << std::setw(60) << " " <<                                                                                           "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "| /-\n";
        std::cout << std::right << " /\"|" << std::setw(30) << "Fixed Code : " << std::left << std::setw(30) <<
                                             std::to_string( GetFAFixedCode() ) + ":" + std::to_string( GetF5FixedCode() ) <<                                   "|/-|\n";
        std::cout << std::right << "/__|" << std::setw(30) << "Man code / model type : " << std::left << std::setw(30) <<
                                             std::to_string( GetManNumber() ) + "/" + std::to_string( GetMType() ) <<                                           "|- |\n";
        std::cout << std::right << "\\__|" << std::setw(60) << " " <<                                                                                           "|- |\n";
        std::cout << std::right << "  ||" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "  ||" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << " /||" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "/_||" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << "\\__|" << std::setw(60) << " " <<                                                                                            "|-_/\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|-/ \n";
        std::cout << std::right << "   |" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "|/\n";
        std::cout << std::setfill(' ');
        std::cout << "\n";
    }

    void V895::ReadConfig( UConfig<V895>& cfg )
    {
        (void)(cfg);
        PrintMessage( Message_t::WARNING, "The configuration cannot be read from V895" );
    }

    void V895::WriteConfig( const UConfig<V895>& cfg )
    {
        for( uint8_t i = 0; i < fChNumber; ++i )
        {
            WriteThreshold( i, cfg.THRESHOLDS.at( i ) );
        }

        WriteMajLevel( cfg.MAJORITY );
        WriteOutWidthH( cfg.WIDTH_H );
        WriteOutWidthL( cfg.WIDTH_L );
        Enable( cfg.MASK );
    }
}
