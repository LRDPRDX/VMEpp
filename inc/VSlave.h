#ifndef V_PLUS_SLAVE_H
#define V_PLUS_SLAVE_H

#include "CAENVMEtypes.h"

#include <cstdint>
#include <string>

/**
 * Every entity of the library is contained in this namespace 
 */
namespace vmeplus
{
    class VMaster;

    /**
     * Represents the SLAVE functional module of the VME standard.
     * Each concrete module must be a child of this class.
     */
    class VSlave
    {
        private :
            VMaster            *fMaster;
            uint32_t            fBaseAddress;
            uint32_t            fRange;

        protected :
            std::string         fName;
            std::string         fFirmware;
            uint16_t            fSerial;

            /**
             * Requests MASTER to perform a READ CYCLE with given parameters
             *
             * @param address VME-address (including the base address)
             * @param data data to read
             * @param dw data width
             * @param am address modifier
             */
            void                ReadRequest( uint32_t address, void *data, CVDataWidth dw = cvD16, CVAddressModifier am = cvA32_U_DATA );

            /**
             * Requests MASTER to perform a WRITE CYCLE with given parameters
             *
             * @param address VME-address (including the base address)
             * @param data data to read
             * @param dw data width
             * @param am address modifier
             */
            void                WriteRequest( uint32_t address, void *data,  CVDataWidth dw = cvD16, CVAddressModifier am = cvA32_U_DATA );

            /**
             * Requests MASTER to perform a BLOCK READ CYCLE with given parameters
             *
             * @param address VME-address (including the base address)
             * @param buffer data to read to [out]
             * @param size number of bytes to be read [in]
             * @param count number of bytes actually read [out]
             * @param dw data width
             * @param am address modifier
             */
            void                BLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVDataWidth dw, CVAddressModifier am = cvA32_U_BLT );

            /**
             * Requests MASTER to perform a MULTIPLEXED BLOCK READ CYCLE with given parameters
             *
             * @param address VME-address (including the base address)
             * @param buffer data to read to [out]
             * @param size number of bytes to be read [in]
             * @param count number of bytes actually read [out]
             * @param am address modifier
             */
            void                MBLTReadRequest( uint32_t address, void *buffer, int size, int *count, CVAddressModifier am = cvA32_U_MBLT );

            /**
             * Requests MASTER to perform a FIFO MULTIPLEXED BLOCK READ CYCLE with given parameters
             *
             * @param address VME-address (including the base address)
             * @param buffer data to read to [out]
             * @param size number of bytes to be read [in]
             * @param count number of bytes actually read [out]
             * @param am address modifier
             */
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
            VSlave( std::string name, uint32_t baseAddress, uint32_t range );
            VSlave( const VSlave &other ) = delete;
            VSlave& operator=( const VSlave &other ) = delete;
            virtual             ~VSlave();

        public :
            uint32_t            GetBaseAddress() const { return fBaseAddress; }
            void                SetBaseAddress( uint32_t newAddress );
            uint32_t            GetRange() const { return fRange; }
            std::string         GetName() const { return fName; }
            std::string         GetFirmware() const { return fFirmware; }
            uint16_t            GetSerialNumber() const { return fSerial; }

            virtual void        Print() const;
            virtual void        Reset();

            void                Release();

            friend class VMaster;
    };
}
#endif
