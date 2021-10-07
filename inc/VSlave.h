#ifndef V_PLUS_SLAVE_H
#define V_PLUS_SLAVE_H

#include "CAENVMEtypes.h"

#include <cstdint>
#include <string>

namespace vmeplus
{
    class VMaster;

    class VSlave
    {
        private :
            VMaster            *fMaster;
            uint32_t            fBaseAddress;
            uint32_t            fRange;

        protected :
            std::string         fFirmware;
            uint16_t            fSerial;

            void                ReadRequest( uint32_t address, void *data, CVDataWidth dw = cvD16, CVAddressModifier am = cvA32_U_DATA );
            void                WriteRequest( uint32_t address, void *data,  CVDataWidth dw = cvD16, CVAddressModifier am = cvA32_U_DATA );
            void                BLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVDataWidth dw, CVAddressModifier am = cvA32_U_BLT );
            void                MBLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVAddressModifier am = cvA32_U_MBLT );
            void                FIFOMBLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVAddressModifier am = cvA32_U_MBLT );

            virtual void        Initialize() = 0;

        protected :
            void                WriteRegister16( uint32_t address, uint16_t data, uint16_t msk = 0xFFFFU );
            uint16_t            ReadRegister16( uint32_t address, uint16_t msk = 0xFFFFU );
            void                WriteRegister32( uint32_t address, uint32_t data, uint32_t msk = 0xFFFFFFFFU);
            uint32_t            ReadRegister32( uint32_t address, uint32_t msk = 0xFFFFFFFFU );
	        virtual void        SetBit16( uint32_t address, uint16_t bit );
            virtual void        ClearBit16( uint32_t address, uint16_t bit );
            virtual bool        GetBit16( uint32_t address, uint16_t bit );

        public :
            VSlave( uint32_t baseAddress, uint32_t range );
            VSlave( const VSlave &other ) = delete;
            VSlave& operator=( const VSlave &other ) = delete;
            virtual             ~VSlave();

        public :
            uint32_t            GetBaseAddress() const { return fBaseAddress; }
            void                SetBaseAddress( uint32_t newAddress );
            uint32_t            GetRange() const { return fRange; }
            std::string         GetFirmware() const { return fFirmware; }
            uint16_t            GetSerialNumber() const { return fSerial; }

            virtual void        Print() const;
            virtual void        Reset();

            void                Release();

            friend class VMaster;
    };
}
#endif
