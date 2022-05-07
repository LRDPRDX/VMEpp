#include "VSlaveAcquisitor.h"

namespace vmepp
{
    void VSlaveAcquisitor::ReadBuffer( VBuffer& buffer )
    {
        size_t nCycles          = HelperReadCycles();
        size_t nWordsInCycle    = gMaxBLT / sizeof( DataWord_t );
        size_t nWords           = nCycles * nWordsInCycle;

        buffer.fData.resize( nWords ); // reallocation only if necessary

        int count;
        size_t writePointer = 0;
        for( size_t i = 0; i < nCycles; ++i )
        {
            FIFOBLTReadRequest( this->GetBufferAddress(), &(buffer.fData.data()[writePointer]), gMaxBLT, &count );
            writePointer += (count == gMaxBLT ? nWordsInCycle : 0); // Only full read is accepted
        }
        buffer.fData.resize( writePointer );
        buffer.fData.shrink_to_fit();
    }

    size_t VSlaveAcquisitor::HelperReadCycles()
    {
        return fReadCycles;
    }

    void VBuffer::Dump( const std::string& fileName )
    {
        // Exceptions : fData is empty, permissions to the file
        std::ofstream f( fileName, std::ios::out | std::ios::binary );
        f.write(reinterpret_cast<char*>(fData.data()), fData.size() * sizeof(DataWord_t) );
        f.close();
    }
}
