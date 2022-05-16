#ifndef V_PLUS_SLAVE_ACQUISITOR_H
#define V_PLUS_SLAVE_ACQUISITOR_H

#include <fstream>
#include <vector>

#include "VSlave.h"
#include "VException.h"

namespace vmepp
{
    class VBuffer;

    class VSlaveAcquisitor : virtual public VSlave
    {
        protected :
            size_t fReadBytes;
            size_t fReadCycles;
            size_t fNEventRequest;

        public :
            VSlaveAcquisitor( uint32_t address, uint32_t range ) :
                VSlave( address, range ),
                fReadBytes( 0 ),
                fReadCycles( 1 ),
                fNEventRequest( 1 )
            {
            }
            ~VSlaveAcquisitor() override = default;

        protected :
            virtual size_t      HelperReadCycles();
            virtual uint32_t    GetBufferAddress() const = 0;

        public :
            void                ReadBuffer( VBuffer& buffer );

            void                SetReadCycles( uint32_t n ) { fReadCycles = (n ? (n <= gMaxNBLT ? n : gMaxNBLT) : 1); }
            size_t              GetReadCycles() { return fReadCycles; }
            void                SetNEventRequest( size_t n ) { fNEventRequest = ((n > 0 ) ? n : 1); }
            size_t              GetNEventRequest() const { return fNEventRequest; }
            size_t              GetReadBytes() const { return fReadBytes; }

    };


    class VBuffer
    {
        private :
            //TODO: consider std::unique_ptr as a data container
            std::vector<DataWord_t>     fData;

        public :
            VBuffer( size_t n = 0 ) :
               fData( n )
            { }

        public :
            size_t GetSize() const { return fData.size(); }
            const DataWord_t& operator [](size_t i) const { return fData[i]; }

            void Dump( const std::string& fileName );

            friend void VSlaveAcquisitor::ReadBuffer( VBuffer& );
    };
}
#endif
