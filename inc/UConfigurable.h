#ifndef V_PLUS_CONFIGURABLE_H
#define V_PLUS_CONFIGURABLE_H

#include "VException.h"

#include <iostream>
#include <fstream>

#include "cereal/archives/json.hpp"


namespace vmepp
{
    template <typename TModuleName>
    struct UConfig
    {
        static_assert(  not std::is_same<TModuleName, TModuleName>::value,
                        "Instantiation of primary template is detected. "
                        "UConfig class template must be specialized for each module." );
    };

    template <typename TModuleName>
    class UConfigurable
    {
        protected :
            static const std::string  fName;

        public :
            virtual void ReadConfig( UConfig<TModuleName>& cfg ) = 0;
            virtual void WriteConfig( const UConfig<TModuleName>& cfg ) = 0;

        public :
            static const std::string&  GetName() { return fName; }
    };

    template<typename TModuleName>
    const std::string UConfigurable<TModuleName>::fName = "Unknown";

    /**
     * Dump a configuration instance to a file
     * @param cfg configuration instance
     * @param path to the file
     * @see ReadConfigFromFile( UConfig<TModuleName>& cfg, const std::string& path )
     */
    template <typename TModuleName, typename TArchive = typename cereal::JSONOutputArchive>
    void WriteConfigToFile( const UConfig<TModuleName>& cfg, const std::string& path )
    {
        try
        {
            std::ofstream os( path );
            TArchive archive( os );
            archive( cereal::make_nvp( TModuleName::GetName() , cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            throw( VException( VError_t::vConfigError, "Writing to " + path ) );
        }
    }

    /**
     * Fill a configuration instance from a file
     * @param cfg configuration instance
     * @param path to the file
     * @see WriteConfigFromFile( const UConfig<TModuleName>& cfg, const std::string& path )
     */
    template <typename TModuleName, typename TArchive = typename cereal::JSONInputArchive>
    void ReadConfigFromFile( UConfig<TModuleName>& cfg, const std::string& path )
    {
        try
        {
            std::ifstream is( path );
            TArchive archive( is );
            archive( cereal::make_nvp( TModuleName::GetName() , cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            throw( VException( VError_t::vConfigError, "Reading from " + path ) );
        }
    }
}
#endif
