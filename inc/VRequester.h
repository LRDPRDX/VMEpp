#ifndef V_PLUS_REQUESTER_H 
#define V_PLUS_REQUESTER_H

#include <CAENVMEtypes.h>

#include "VModule.h"

namespace vmepp
{
    class VRequester : virtual public VModule
    {
        public :
            VRequester() = default;
            VRequester( const VRequester &other ) = delete;
            VRequester& operator=( const VRequester &other ) = delete;
            virtual ~VRequester() = default;

        public :
            virtual void                WriteRequesterType( CVRequesterTypes type ) = 0;
            virtual CVRequesterTypes    ReadRequesterType() = 0;
            virtual void                WriteReleaseType( CVReleaseTypes type ) = 0;
            virtual CVReleaseTypes      ReadReleaseType() = 0;
            virtual void                WriteBusReqLevel( CVBusReqLevels level ) = 0;
            virtual CVBusReqLevels      ReadBusReqLevel() = 0;
    };
}
#endif
