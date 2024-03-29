#include "modules/V6533N.h"
#include "VException.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

namespace vmepp
{
    V6533N::V6533N( uint32_t baseAddress, uint32_t range ) :
        VSlave(  baseAddress, range ),

        fChannels( 0 ),
        fDescription( "N/A" ),
        fModel( "N/A" ),
        fVMEFirmware( "N/A" )
    {
    }

    template<>
    const std::string UConfigurable<V6533N>::fName = "V6533N";

    void V6533N::Initialize()
    {
        PrintMessage( Message_t::INFO, "Initializing " + fName + "..." );

        ReadSerialNumber();
        ReadFWRel();
        ReadNChannels();
        ReadDescription();
        ReadModel();
        ReadVMEFWRel();

        PrintMessage( Message_t::INFO, "Initializing " + fName + "...OK" );
        return;
    }

    //****************
    //Board parameters
    //****************
    float V6533N::ReadVMax()
    {
        return (float)ReadRegister16( V6533N_VMAX ); //[V]
    }

    float V6533N::ReadIMax()
    {
        return (float)ReadRegister16( V6533N_IMAX );// [uA]
    }


    uint16_t V6533N::ReadStatus()
    {
        return ReadRegister16( V6533N_STATUS );
    }


    uint16_t V6533N::ReadFWRel()
    {
        uint16_t fwrel = ReadRegister16( V6533N_FWREL );

        fFirmware = std::to_string( (fwrel & V6533N_MC_MJR_REL_MSK) >> 8 ) + "." +
                    std::to_string(  fwrel & V6533N_MC_MNR_REL_MSK );

        return fwrel;
    }


    //******************
    //Channel parameters
    //******************
    float V6533N::ReadVoltage( uint16_t ch )
    {
        return (float)ReadRegister16( V6533N_VMON(ch % fChNumber) ) * 0.1;// in V
    }

    float V6533N::ReadVoltageSet( uint16_t ch )
    {
        return (float)ReadRegister16( V6533N_VSET(ch % fChNumber) ) * 0.1;// in V
    }

    void V6533N::WriteVoltage( uint16_t ch, float voltage )
    {
        uint16_t rawV = std::round( 10 * voltage );
        WriteRegister16( V6533N_VSET(ch % fChNumber), rawV );
    }

    float V6533N::ReadCurrent( uint16_t ch, V6533N::IMonRange_t range )
    {
        //Return value in [uA]
        uint16_t rawI;
        float scale;
        //Save old range
        V6533N::IMonRange_t old_range = ReadIMonRange( ch % fChNumber );
        WriteIMonRange( ch % fChNumber, range );

        switch( range )
        {
            case( V6533N::IMonRange_t::RANGE_HIGH ) :
                rawI = ReadRegister16( V6533N_IMONH(ch % fChNumber) );
                scale = 5e-2;//0.05 [uA]
                break;
            case( V6533N::IMonRange_t::RANGE_LOW ) :
                rawI = ReadRegister16( V6533N_IMONL(ch % fChNumber) );
                scale = 5e-3;//0.005 [uA]
                break;
        }
        //Restore old range
        WriteIMonRange( ch % fChNumber, old_range );

        return (float)rawI * scale;
    }

    float V6533N::ReadCurrentSet( uint16_t ch )
    {
        return ReadRegister16( V6533N_ISET(ch % fChNumber) ) * 5e-2;// in uA
    }

    void V6533N::WriteCurrent( uint16_t ch, float current )
    {
        uint16_t rawI = current / 5e-2;// current in uA
        WriteRegister16( V6533N_ISET(ch % fChNumber), rawI );
    }

    bool V6533N::ReadEnable( uint16_t ch )
    {
        return ReadRegister16( V6533N_PW(ch % fChNumber) );
    }

    void V6533N::WriteEnable( uint16_t ch, bool en )
    {
        uint16_t data = en ? 1 : 0;
        WriteRegister16( V6533N_PW(ch % fChNumber), data );
    }

    uint16_t V6533N::ReadStatus( uint16_t ch )
    {
        return ReadRegister16( V6533N_CHSTATUS(ch % fChNumber) );
    }

    float V6533N::ReadTripTime( uint16_t ch )
    {
        return (float)ReadRegister16( V6533N_TRIP_TIME(ch % fChNumber) ) * 0.1;
    }

    void V6533N::WriteTripTime( uint16_t ch, float ttime )
    {
        uint16_t raw_data = std::round( ttime * 10 );
        WriteRegister16( V6533N_TRIP_TIME(ch % fChNumber), raw_data );
    }

    float V6533N::ReadSWVMax( uint16_t ch )
    {
        return (float)ReadRegister16( V6533N_SVMAX(ch % fChNumber) ) * 0.1;
    }

    void V6533N::WriteSWVMax( uint16_t ch, float voltage )
    {
        uint16_t raw_data = voltage * 10;
        WriteRegister16( V6533N_SVMAX(ch % fChNumber), raw_data );
    }

    uint16_t V6533N::ReadRampDown( uint16_t ch )
    {
        return ReadRegister16( V6533N_RAMP_DOWN(ch % fChNumber) );
    }

    void V6533N::WriteRampDown( uint16_t ch, uint16_t rampd )
    {
        WriteRegister16( V6533N_RAMP_DOWN(ch % fChNumber), rampd );
    }

    uint16_t V6533N::ReadRampUp( uint16_t ch )
    {
        return ReadRegister16( V6533N_RAMP_UP(ch % fChNumber) );
    }

    void V6533N::WriteRampUp( uint16_t ch, uint16_t rampu )
    {
        WriteRegister16( V6533N_RAMP_UP(ch % fChNumber), rampu );
    }

    bool V6533N::ReadPWDown( uint16_t ch )
    {
        return not ReadRegister16( V6533N_PWDOWN(ch % fChNumber) );
    }

    void V6533N::WritePWDown( uint16_t ch, bool kill )
    {
        uint16_t data = kill ? 0 : 1;
        WriteRegister16( V6533N_PWDOWN(ch % fChNumber), data );
    }

    V6533N::IMonRange_t V6533N::ReadIMonRange( uint16_t ch )
    {
        uint16_t data = ReadRegister16( V6533N_IMON_RANGE(ch % fChNumber) );
        return (data > 0) ? V6533N::IMonRange_t::RANGE_LOW : V6533N::IMonRange_t::RANGE_HIGH;
    }

    void V6533N::WriteIMonRange( uint16_t ch, IMonRange_t range )
    {
        uint16_t data = (range == V6533N::IMonRange_t::RANGE_HIGH) ? 0 : 1;
        WriteRegister16( V6533N_IMON_RANGE(ch % fChNumber), data );
    }

    V6533N::Polarity_t V6533N::ReadPolarity( uint16_t ch )
    {
        uint16_t data = ReadRegister16( V6533N_POLARITY(ch % fChNumber) );
        return (data > 0) ? V6533N::Polarity_t::POSITIVE : V6533N::Polarity_t::NEGATIVE;
    }

    int16_t V6533N::ReadTemperature( uint16_t ch )
    {
        return ReadRegister16( V6533N_TEMPERATURE(ch % fChNumber) );
    }

    //*******************
    //Board configuration
    //*******************
    uint16_t V6533N::ReadNChannels()
    {
        fChannels = ReadRegister16( V6533N_CHNUM );
        return fChannels;
    }

    std::string V6533N::ReadDescription()
    {
        uint16_t buffer[10];
        for( uint32_t i = 0; i < 10; i++ )
        {
            buffer[i] = ReadRegister16( V6533N_DESCR + 0x0002U * i );
        }
        fDescription = std::string( reinterpret_cast<char*>( buffer ) );
        return fDescription;
    }

    std::string V6533N::ReadModel()
    {
        uint16_t buffer[8];
        for( uint32_t i = 0; i < 4; i++ )
        {
            buffer[i] = ReadRegister16( V6533N_MODEL + 0x0002U * i );
        }
        fModel = std::string( reinterpret_cast<char*>( buffer ) );
        return fModel;
    }

    uint16_t V6533N::ReadSerialNumber()
    {
        fSerial = ReadRegister16( V6533N_SERNUM );
        return fSerial;
    }

    uint16_t V6533N::ReadVMEFWRel()
    {
        int16_t data = ReadRegister16( V6533N_VME_FWREL );

        fVMEFirmware = std::to_string( (data & V6533N_VME_MJR_REL_MSK) >> 8 ) + "." +
                       std::to_string(  data & V6533N_VME_MNR_REL_MSK );
        return data;
    }

    //******************
    //Additional methods
    //******************
    void V6533N::Print() const
    {
        std::cout << std::right << "\n";
        std::cout << std::right << "     /" << std::setfill('=') << std::setw(60) << "=" <<                                                                        "/-\n";
        std::cout << std::right << "    /" << std::setfill(' ') << std::setw(60) << " " <<                                                                        "/-|\n";
        std::cout << std::right << "   /" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "/- |\n";
        std::cout << std::setfill(' ');
        std::cout << std::right << "   |" << std::setw(30) << fName << std::setw(30) << " " <<                                                                  "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << " __|" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "/__|" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << " __|" << std::setw(60) << " " <<                                                                                            "|-_|\n";
        std::cout << std::right << "/__|" << std::setw(30) << "Serial Number : " << std::left << std::setw(30) << GetSerialNumber() <<                          "|- |\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << " __|" << std::setw(30) << "Number of channels : " << std::left << std::setw(30) << GetNChannels() <<                        "|  |\n";
        std::cout << std::right << "/__|" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "| /-\n";
        std::cout << std::right << " __|" << std::setw(30) << "MC FW release : " << std::left << std::setw(30) << GetFirmware() <<                              "|/-|\n";
        std::cout << std::right << "/__|" << std::setw(30) << "FW release : " << std::left << std::setw(30) << GetVMEFirmware() <<                              "|- |\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << " __|" << std::setw(30) << "Description : " << std::left << std::setw(30) << GetDescription() <<                             "|- |\n";
        std::cout << std::right << "/__|" << std::setw(30) << "Model : " << std::left << std::setw(30) << GetModel() <<                                         "|- |\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << " __|" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << "/__|" << std::setw(60) << " " <<                                                                                            "|-_/\n";
        std::cout << std::right << "L__|" << std::setw(60) << " " <<                                                                                            "|-/ \n";
        std::cout << std::right << "   |" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "|/\n";
        std::cout << std::setfill(' ');
        std::cout << "\n";
    }

    void V6533N::ReadConfig( UConfig<V6533N>& cfg )
    {
        for( uint8_t i = 0; i < fChNumber; ++i )
        {
            IMonRange_t iMon = ReadIMonRange( i );

            cfg.CHANNELS.at( i ).V_SET = ReadVoltageSet( i );
            cfg.CHANNELS.at( i ).I_SET = ReadCurrentSet( i );
            cfg.CHANNELS.at( i ).TRIP_TIME = ReadTripTime( i );
            cfg.CHANNELS.at( i ).SW_MAX = ReadSWVMax( i );
            cfg.CHANNELS.at( i ).RAMP_UP = ReadRampUp( i );
            cfg.CHANNELS.at( i ).RAMP_DOWN = ReadRampDown( i );
            cfg.CHANNELS.at( i ).PW_DOWN = ReadPWDown( i );
            cfg.CHANNELS.at( i ).IMON_RANGE = iMon;
        }
    }

    void V6533N::WriteConfig( const UConfig<V6533N>& cfg )
    {
        for( uint8_t i = 0; i < fChNumber; ++i )
        {
            WriteVoltage( i, cfg.CHANNELS.at( i ).V_SET );
            WriteCurrent( i, cfg.CHANNELS.at( i ).I_SET );
            WriteTripTime( i, cfg.CHANNELS.at( i ).TRIP_TIME );
            WriteSWVMax( i, cfg.CHANNELS.at( i ).SW_MAX );
            WriteRampUp( i, cfg.CHANNELS.at( i ).RAMP_UP );
            WriteRampDown( i, cfg.CHANNELS.at( i ).RAMP_DOWN );
            WritePWDown( i, cfg.CHANNELS.at( i ).PW_DOWN );
            WriteIMonRange( i, cfg.CHANNELS.at( i ).IMON_RANGE );
        }
    }

    V6533N::MonitorData V6533N::ReadMonitor()
    {
        V6533N::MonitorData m;

        m.V_MAX     = ReadVMax();
        m.I_MAX     = ReadIMax();
        m.STATUS    = ReadStatus();

        for( uint8_t i = 0; i < fChNumber; ++i )
        {
            m.CHANNELS.at( i ).VOLTAGE = ReadVoltage( i );
            m.CHANNELS.at( i ).CURRENT = ReadCurrent( i, ReadIMonRange( i ) );
            m.CHANNELS.at( i ).STATUS  = ReadStatus( i );
            m.CHANNELS.at( i ).TEMP    = ReadTemperature( i );
        }

        return m;
    }
}
