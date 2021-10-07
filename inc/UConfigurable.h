#ifndef V_PLUS_CONFIGURABLE_H
#define V_PLUS_CONFIGURABLE_H

#include "VException.h"

#include <iostream>
#include <fstream>

#include "cereal/archives/json.hpp"


namespace vmeplus
{
    template <typename T>
    struct UConfig;

    template <typename T>
    class UConfigurable
    {
        protected :
            static std::string  fName;

        public :
            virtual void ReadConfig( UConfig<T>& cfg ) = 0;
            virtual void WriteConfig( const UConfig<T>& cfg ) = 0;

        public :
            static std::string  GetName() { return fName; }
    };

    template <typename T, typename Archive = typename cereal::JSONOutputArchive>
    void WriteConfigToFile( const UConfig<T>& cfg, const std::string& path )
    {
        try
        {
            std::ofstream os( path );
            Archive archive( os );
            archive( cereal::make_nvp( T::GetName() , cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            throw( VException( VError_t::vConfigError, "Writing to " + path ) );
        }
    }

    template <typename T, typename Archive = typename cereal::JSONInputArchive>
    void ReadConfigFromFile( UConfig<T>& cfg, const std::string& path )
    {
        try
        {
            std::ifstream is( path );
            Archive archive( is );
            archive( cereal::make_nvp( T::GetName() , cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            throw( VException( VError_t::vConfigError, "Reading from " + path ) );
        }
    }
}
#endif
