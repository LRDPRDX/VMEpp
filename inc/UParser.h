#pragma once

#include "VModule.h"
#include "VSlaveAcquisitor.h"

namespace vmepp
{
    template<typename TModuleName>
    class UEvent;

    class UParser
    {
        private :
            size_t fNEventsRead;
            size_t fCurrentEvent;
            size_t fCurrentIndex;

        public :
            UParser() :
                fNEventsRead( 0 ),
                fCurrentEvent( 0 ),
                fCurrentIndex( 0 )
            { }

        public :
            template<typename TModuleName>
            bool   GetEvent( UEvent<TModuleName> &event, const VBuffer &buffer );

            size_t GetNEventsRead() const { return fNEventsRead; }
            size_t GetCurrentIndex() const { return fCurrentIndex; }
            size_t GetCurrentEvent() const { return fCurrentEvent; }

            void ResetIndex()
                 {
                   fCurrentEvent = 0;
                   fCurrentIndex = 0;
                   fNEventsRead = 0;
                 }
    };

    template<typename TModuleName>
    bool UParser::GetEvent( UEvent<TModuleName> &event, const VBuffer &buffer )
    {
        if( fCurrentIndex >= buffer.GetSize() )
        {
            return false;
        }

        if( event.Fill( fCurrentIndex, buffer ) )
        {
            fCurrentIndex = event.GetStop() + 1;
            fNEventsRead++;
            return true;
        }

        return false;
    }
}
