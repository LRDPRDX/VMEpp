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

        public :
            VSlaveAcquisitor( uint32_t address, uint32_t range ) :
                VSlave( address, range ),
                fReadBytes( 0 ),
                fReadCycles( 1 )
            {
            }
            ~VSlaveAcquisitor() override = default;

        public :
            virtual uint32_t    GetBufferAddress() const = 0;

        public :
            void                ReadBuffer( VBuffer& buffer );
            void                SetReadCycles( uint32_t n ) { fReadCycles = (n ? (n <= gMaxNBLT ? n : gMaxNBLT) : 1); }
            size_t              GetReadCycles() { return fReadCycles; }
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

            friend void VSlaveAcquisitor::ReadBuffer( VBuffer& );
    };
}
#endif
