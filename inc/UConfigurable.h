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
        public :
            virtual void ReadConfig( UConfig<T>& cfg ) = 0;
            virtual void WriteConfig( const UConfig<T>& cfg ) = 0;
    };

    template <typename T, typename Archive = typename cereal::JSONOutputArchive>
    void WriteConfigToFile( const UConfig<T>& cfg, const std::string& path )
    {
        try
        {
            std::ofstream os( path );
            Archive archive( os );
            archive( CEREAL_NVP( cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            std::cout << e.what() << "\n";
        }
    }

    template <typename T, typename Archive = typename cereal::JSONInputArchive>
    void ReadConfigFromFile( UConfig<T>& cfg, const std::string& path )
    {
        try
        {
            std::ifstream is( path );
            Archive archive( is );
            archive( CEREAL_NVP( cfg ) );
        }
        catch( const cereal::Exception& e )
        {
            std::cout << e.what() << "\n";
        }
    }
}
#endif
