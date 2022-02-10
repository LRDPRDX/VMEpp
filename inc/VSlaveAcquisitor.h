#ifndef V_PLUS_SLAVE_ACQUISITOR_H
#define V_PLUS_SLAVE_ACQUISITOR_H

#include <memory>
#include <fstream>

#include "VSlave.h"
#include "VException.h"

namespace vmepp
{
    template<typename TModuleName>
    class UEvent;

    template<typename TModuleName>
    class VSlaveAcquisitor : virtual public VSlave
    {
        protected :
            std::unique_ptr<DataWord_t[]> fBuffer;
            size_t                        fBufferSize;
            size_t                        fReadBytes;
            size_t                        fNEventsRead;
            size_t                        fCurrentEvent;
            size_t                        fCurrentIndex;
            size_t                        fReadCycles;

        public :
            VSlaveAcquisitor( uint32_t address, uint32_t range ) :
                VSlave( address, range ),

                fBuffer( nullptr ),
                fBufferSize( 0 ),
                fReadBytes( 0 ),
                fNEventsRead( 0 ),
                fCurrentEvent( 0 ),
                fCurrentIndex( 0 ),
                fReadCycles( 1 )
            {
            }
            ~VSlaveAcquisitor() override = default;

        public :
            void                AllocateBuffer();
            size_t              ReadBuffer();
            void                DropBuffer( const std::string& fileName );
            bool                GetEvent( UEvent<TModuleName> &event );

            virtual bool        GetEventAt( size_t index, UEvent<TModuleName> &event ) const = 0;
            virtual uint32_t    GetBufferAddress() const = 0;

            void                SetReadCycles( size_t n ) { fReadCycles = (n ? (n <= gMaxNBLT ? n : gMaxNBLT) : 1); }
            size_t              GetReadCycles() { return fReadCycles; }

            size_t              GetBufferSize() const { return fBufferSize; }
            size_t              GetNEventsRead() const { return fNEventsRead; }
            size_t              GetReadBytes() const { return fReadBytes; }
            size_t              GetCurrentIndex() const { return fCurrentIndex; }
            size_t              GetCurrentEvent() const { return fCurrentEvent; }

            void                ResetIndex()
                                {
                                  fCurrentEvent = 0;
                                  fCurrentIndex = 0;
                                  fNEventsRead = 0;
                                }
    };

    template<typename TModuleName>
    void VSlaveAcquisitor<TModuleName>::AllocateBuffer()
    {
        this->ResetIndex();

        if( fBuffer )
        {
            PrintMessage( Message_t::WARNING, "Trying to reallocate buffer (not nullptr)" );
        }
        try
        {
            fBufferSize = fReadCycles * gMaxBLT / sizeof(DataWord_t);
            if( fBufferSize )
            {
                fBuffer.reset( new DataWord_t[fBufferSize] );
            }
        }
        catch( std::bad_alloc &e )
        {
            fBuffer.reset();
            fBufferSize = 0;
            throw VException( VError_t::vBuffAllocFailed, "from V1785N::AllocateBuffer" );
        }
    }

    template<typename TModuleName>
    size_t VSlaveAcquisitor<TModuleName>::ReadBuffer()
    {
        this->ResetIndex();

        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "Read buffer : buffer is nullptr. Forgot to allocate?" );
            return 0;
        }
        int count;
        FIFOBLTReadRequest( this->GetBufferAddress(), fBuffer.get(), fBufferSize * sizeof(DataWord_t), &count );
        return (fReadBytes = (count > 0) ? count : 0);
    }

    template<typename TModuleName>
    bool VSlaveAcquisitor<TModuleName>::GetEvent( UEvent<TModuleName> &event )
    {
        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "GetEventAt : buffer is nullptr. Forgot to allocate?" );
            return false;
        }

        if( fCurrentIndex >= fReadBytes / sizeof(DataWord_t) )
        {
            return false;
        }

        if( GetEventAt( fCurrentIndex, event ) )
        {
            fCurrentIndex = event.GetStop() + 1;
            fNEventsRead++;
            return true;
        }

        return false;
    }

    template<typename TModuleName>
    void VSlaveAcquisitor<TModuleName>::DropBuffer( const std::string& path )
    {
        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "DropBuffer() : buffer is nullptr. Forgot to allocate?" );
            return;
        }

        std::ofstream file;
        file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            file.open( path, std::ios::binary | std::ios::trunc );
            file.write((char*)fBuffer.get(), fReadBytes);
        }
        catch( const std::ofstream::failure& e )
        {
            throw( VException( VError_t::vAccessError, "Couldn't write to file " + path ) );
        }
    }
}

#endif
