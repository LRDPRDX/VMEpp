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

    void VBuffer::Dump( const std::string& fileName )
    {
        // Exceptions : fData is empty, permissions to the file
        std::ofstream f( fileName, std::ios::out | std::ios::binary );
        f.write(reinterpret_cast<char*>(fData.data()), fData.size() * sizeof(DataWord_t) );
        f.close();
    }
}
