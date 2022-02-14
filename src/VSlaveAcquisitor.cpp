#include "VSlaveAcquisitor.h"

namespace vmepp
{
    void VSlaveAcquisitor::ReadBuffer( VBuffer& buffer )
    {
        size_t bufSize = fReadCycles * gMaxBLT / sizeof(DataWord_t);

        buffer.fData.resize( bufSize ); // reallocation only if needed

        int count;
        FIFOBLTReadRequest( this->GetBufferAddress(), buffer.fData.data(), bufSize * sizeof(DataWord_t), &count );
        buffer.fData.resize( count >= 0 ? count : 0 );
        buffer.fData.shrink_to_fit();
    }
}
