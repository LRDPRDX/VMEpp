#ifndef V_PLUS_V6533N_H
#define V_PLUS_V6533N_H

//*******************************************************************
//
// Description :
//      Header for the CAEN V6533N module.
//      V6533N is a 6 channel 4kV/3 mA VME HV Power Supply Module
// Author :
//      Sikach Bogdan
// Location :
//      Budker Institute of Nuclear Physics. Room 210/2, Phone 51-57
// Feedback :
//      paradox1859@gmail.com
//
//*******************************************************************
//
//
//*******************************************************************
// Comments description
// D32, D16, ...    = Data size
// A32, A24, ...    = Address modifier (however, it's always A32 here)
// R, W, RW         = Read, Write or Read-Write access modes
// Aux              = Auxiliary data
//*******************************************************************

//Board parameters
#define     V6533N_VMAX             0x0050UL//A32/D16 R
#define     V6533N_IMAX             0x0054UL//A32/D16 R
#define     V6533N_STATUS           0x0058UL//A32/D16 R

#define     V6533N_FWREL            0x005CUL//A32/D16 R
#define     V6533N_MC_MNR_REL_MSK   0x00FFUL//Aux
#define     V6533N_MC_MJR_REL_MSK   0xFF00UL//Aux

//Channel parameters
#define     V6533N_VSET(x)         ((0x0080UL * (x)) + 0x0080UL)//A32/D16 RW
#define     V6533N_ISET(x)         ((0x0080UL * (x)) + 0x0084UL)//A32/D16 RW
#define     V6533N_VMON(x)         ((0x0080UL * (x)) + 0x0088UL)//A32/D16 R
#define     V6533N_IMONH(x)        ((0x0080UL * (x)) + 0x008CUL)//A32/D16 R
#define     V6533N_PW(x)           ((0x0080UL * (x)) + 0x0090UL)//A32/D16 RW
#define     V6533N_CHSTATUS(x)     ((0x0080UL * (x)) + 0x0094UL)//A32/D16 R
#define     V6533N_TRIP_TIME(x)    ((0x0080UL * (x)) + 0x0098UL)//A32/D16 RW
#define     V6533N_SVMAX(x)        ((0x0080UL * (x)) + 0x009CUL)//A32/D16 RW
#define     V6533N_RAMP_DOWN(x)    ((0x0080UL * (x)) + 0x00A0UL)//A32/D16 RW
#define     V6533N_RAMP_UP(x)      ((0x0080UL * (x)) + 0x00A4UL)//A32/D16 RW
#define     V6533N_PWDOWN(x)       ((0x0080UL * (x)) + 0x00A8UL)//A32/D16 RW
#define     V6533N_POLARITY(x)     ((0x0080UL * (x)) + 0x00ACUL)//A32/D16 R
#define     V6533N_TEMPERATURE(x)  ((0x0080UL * (x)) + 0x00B0UL)//A32/D16 R
#define     V6533N_IMON_RANGE(x)   ((0x0080UL * (x)) + 0x00B4UL)//A32/D16 RW
#define     V6533N_IMONL(x)        ((0x0080UL * (x)) + 0x00B8UL)//A32/D16 R

//Board configuration
#define     V6533N_CHNUM            0x8100UL//A32/D16 R  
#define     V6533N_DESCR            0x8102UL//A32/D16 R 
#define     V6533N_MODEL            0x8116UL//A32/D16 R 
#define     V6533N_SERNUM           0x811EUL//A32/D16 R 

#define     V6533N_VME_FWREL        0x8120UL//A32/D16 R
#define     V6533N_VME_MNR_REL_MSK  0x00FFUL//Aux
#define     V6533N_VME_MJR_REL_MSK  0xFF00UL//Aux

#define     V6533N_LUB              0x8121UL//Aux, The Last Used Byte of address (inclusive)


#include "VSlave.h"
#include "UConfigurable.h"

#include <array>
#include <string>

namespace vmepp
{
    class V6533N : public VSlave, public UConfigurable<V6533N>
    {
        protected :
            static uint8_t constexpr fChNumber = 0x06U;   // 6

        public :
            static uint8_t constexpr GetChNumber() { return fChNumber; }

        protected :
            virtual void    Initialize() override;

        protected :
            uint16_t        fChannels;
            std::string     fDescription;
            std::string     fModel;
            std::string     fVMEFirmware;

        public :
            V6533N( uint32_t baseAddress, uint32_t range = V6533N_LUB );
            ~V6533N() override = default;

        public :
            enum class      Polarity_t { NEGATIVE = 0, POSITIVE = 1 };
            enum class      IMonRange_t { RANGE_HIGH = 0, RANGE_LOW = 1 };

            //Board Parameters
            float           ReadVMax();//throws
            float           ReadIMax();//throws
            uint16_t        ReadStatus();//throws
            uint16_t        ReadFWRel();//throws

            //Channel parameters
            float           ReadVoltage( uint16_t ch );//throws
            float           ReadVoltageSet( uint16_t ch );//throws
            void            WriteVoltage( uint16_t ch, float voltage );//throws

            float           ReadCurrent( uint16_t ch, IMonRange_t range );//throws
            float           ReadCurrentSet( uint16_t ch );//throws
            void            WriteCurrent( uint16_t ch, float current );//throws

            bool            ReadEnable( uint16_t ch );//throws
            void            WriteEnable( uint16_t ch, bool status );//throws

            uint16_t        ReadStatus( uint16_t ch );//throws

            float           ReadTripTime( uint16_t ch );//throws
            void            WriteTripTime( uint16_t ch, float ttime );//throws

            float           ReadSWVMax( uint16_t ch );//throws
            void            WriteSWVMax( uint16_t ch, float vmax );//throws

            uint16_t        ReadRampDown( uint16_t ch );//throws
            void            WriteRampDown( uint16_t ch, uint16_t rampd );//throws

            uint16_t        ReadRampUp( uint16_t ch );//throws
            void            WriteRampUp( uint16_t ch, uint16_t rampu );//throws

            bool            ReadPWDown( uint16_t ch );//throws
            void            WritePWDown( uint16_t ch, bool kill );//throws

            IMonRange_t     ReadIMonRange( uint16_t ch );//throws 
            void            WriteIMonRange( uint16_t ch, IMonRange_t range ); //throws
            //float ReadIMonH( uint16_t ch );//throws
            //float ReadIMonL( uint16_t ch );//throws

            Polarity_t      ReadPolarity( uint16_t ch );//throws
            int16_t         ReadTemperature( uint16_t ch );//throws

            ////Board configuration
            uint16_t        ReadNChannels();//throws 
            std::string     ReadDescription();//throws
            std::string     ReadModel();//throws
            uint16_t        ReadSerialNumber();//throws
            uint16_t        ReadVMEFWRel();//throws

        //STATE methods
        public :
            uint16_t        GetNChannels() const { return fChannels; }
            std::string     GetDescription() const { return fDescription; }
            std::string     GetModel() const { return fModel; }
            std::string     GetVMEFirmware() const { return fVMEFirmware; }

        public :
            virtual void    Print() const override;

        public :
            struct MonitorData
            {
                struct Channel
                {
                    float VOLTAGE;
                    float CURRENT;
                    uint16_t STATUS;
                    int16_t TEMP;
                };

                float V_MAX;
                float I_MAX;
                uint16_t STATUS;
                std::array<Channel, fChNumber> CHANNELS;
            };

            void    ReadConfig( UConfig<V6533N>& config ) override;
            void    WriteConfig( const UConfig<V6533N>& config ) override;

            V6533N::MonitorData ReadMonitor();
    };// V6533N

    template<>
    const std::string UConfigurable<V6533N>::fName;

    template<>
    struct UConfig<V6533N>
    {
        struct Channel
        {
            float               V_SET;
            float               I_SET;
            float               TRIP_TIME;
            float               SW_MAX;
            uint16_t            RAMP_UP;
            uint16_t            RAMP_DOWN;
            bool                PW_DOWN;
            V6533N::IMonRange_t IMON_RANGE;

            Channel() :
                V_SET{},
                I_SET{},
                TRIP_TIME{},
                SW_MAX{},
                RAMP_UP{},
                RAMP_DOWN{},
                PW_DOWN{},
                IMON_RANGE{}
            {
            }

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "voltage_set", V_SET ),
                    cereal::make_nvp( "current_set", I_SET ),
                    cereal::make_nvp( "trip_time", TRIP_TIME ),
                    cereal::make_nvp( "sw_max_voltage", SW_MAX ),
                    cereal::make_nvp( "ramp_up", RAMP_UP ),
                    cereal::make_nvp( "ramp_down", RAMP_DOWN ),
                    cereal::make_nvp( "power_down", PW_DOWN ),
                    cereal::make_nvp( "current_mon_range", IMON_RANGE ) );
            }
        };

        std::array<Channel, V6533N::GetChNumber()> CHANNELS;

        template <class TArchive>
        void serialize( TArchive& ar )
        {
            ar( cereal::make_nvp( "channels", CHANNELS ) );
        }
    };// UConfig<V6533N>
}

#endif
