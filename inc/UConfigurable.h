#ifndef V_PLUS_UCONFIGURABLE_H
#define V_PLUS_UCONFIGURABLE_H

#include "VException.h"

#include <iostream>


namespace vmeplus
{
    template <typename T>
    struct UConfig;

    template <typename T>
    class UConfigurable
    {
        public :
            virtual void ReadConfig( UConfig<T>& cfg ) = 0;
            virtual void WriteConfig( const UConfig<T>& cfg ) = 0;
    };
}
#endif
