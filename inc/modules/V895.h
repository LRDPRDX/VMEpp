#ifndef V_PLUS_V895_H
#define V_PLUS_V895_H

//*******************************************************************
//
// Description :
//      Header for the CAEN V895 module.
//      V895 is a 16 channel Leading Edge Discriminator 
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

//Channel config
#define     V895_THRESHOLD(x)   (0x0002UL * (x))//A32/D16 W
#define     V895_THRESHOLD_MSK  0x00FFU//Aux

//Board general config
#define     V895_OUT_WIDTH_L    0x0040UL//A32/D16 W 
#define     V895_OUT_WIDTH_H    0x0042UL//A32/D16 W 
#define     V895_OUT_WIDTH_MSK  0x00FFU//Aux

#define     V895_MAJ_THRESHOLD  0x0048UL//A32/D16 W
#define     V895_MAJ_THR_MSK    0x00FFU//Aux

#define     V895_PAT_INHIBIT    0x004AUL//A32/D16 W
#define     V895_TEST_PULSE     0x004CUL//A32/D16 W

//Board information
#define     V895_FIXED_CODE     0x00FAUL//A32/D16 R
#define     V895_F5_MSK         0x00FFUL//Aux
#define     V895_FA_MSK         0xFF00UL//Aux

#define     V895_MAN_MTYPE      0x00FCUL//A32/D16 R
#define     V895_MTYPE_MSK      0x03FFUL//Aux 
#define     V895_MAN_MSK        0xFC00UL//Aux 

#define     V895_VER_SER        0x00FEUL//A32/D16 R
#define     V895_SERIAL_MSK     0x0FFFUL//Aux
#define     V895_VER_MSK        0xF000UL//Aux

//Not specified
//#define     V895_N_CHANNELS     0x0010U//Aux
#define     V895_LUB            0x00FFUL//Aux, The Lust Used Byte


#include "VSlave.h"
#include "UConfigurable.h"

namespace vmeplus
{
    class V895 : public VSlave, public UConfigurable
    {
        protected :
            static uint8_t const fChNumber = 0x10U;   // 16

        public :
            static uint8_t GetChNumber() { return fChNumber; }

        protected :
            virtual void    Initialize() override;

        protected :
            uint16_t        fVersionSerial;
            uint16_t        fManMType;
            uint16_t        fFixedCode;

        public :
            V895( uint32_t baseAddress, uint32_t range = V895_LUB );
            virtual         ~V895();

        public :
            //Channel configuration
            void            WriteThreshold( uint8_t ch, uint8_t thr );
            //Board general configuration
            uint16_t        GetValueFromWidth( uint16_t width );
            void            WriteOutWidth( uint16_t width );
            void            WriteOutWidthH( uint16_t width );
            void            WriteOutWidthL( uint16_t width );
            void            WriteMajLevel( uint16_t level );
            void            Enable( uint16_t mask );
            void            EnableOnly( uint8_t ch );
            void            SendTest();
            //Board information
            uint16_t        ReadVerSer();
            uint16_t        ReadManMType();
            uint16_t        ReadFixedCode();

        //STATE methods 
        public :
            uint16_t        GetVersionSerial() const { return fVersionSerial; }
            uint16_t        GetVersion() const { return (fVersionSerial & V895_VER_MSK) >> 12; }
            //Getting the serial number is inherited from the VSlave class

            uint16_t        GetManMType() const { return fManMType; }
            uint16_t        GetManNumber() const { return (fManMType & V895_MAN_MSK) >> 10; }
            uint16_t        GetMType() const { return fManMType & V895_MTYPE_MSK; }

            uint16_t        GetFixedCode() const { return fFixedCode; } 
            uint16_t        GetFAFixedCode() const { return (fFixedCode & V895_FA_MSK) >> 8; }
            uint16_t        GetF5FixedCode() const { return fFixedCode & V895_F5_MSK; }

            uint16_t        GetNChannels() const { return 16; }

        public :
            virtual void    Print() const override;

        public :
            virtual void    ReadConfig( nlohmann::json &config ) override;
            virtual void    WriteConfig( const nlohmann::json &config ) override;
    };
}
#endif
