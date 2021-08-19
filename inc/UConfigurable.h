#ifndef V_PLUS_CONFIGURABLE_H
#define V_PLUS_CONFIGURABLE_H

#include "VException.h"

#include <nlohmann/json.hpp>
#include <iostream>


namespace vmeplus
{
    using json = nlohmann::json;

    // Curiously recurring template pattern
    template <typename T>
    class UConfigurable
    {
        protected :
            static json   fDefaultConfig;

            virtual void ReadConfigImpl( nlohmann::json &config ) = 0;
            virtual void WriteConfigImpl( const nlohmann::json &config ) = 0;

        public :
            UConfigurable() {};
            virtual ~UConfigurable() {};

            void ReadConfig( nlohmann::json &config );
            void WriteConfig( const nlohmann::json &config );

            static json GetDefaultConfig() { return fDefaultConfig; }
            static bool Validate( const json& source );
    };

    template<typename T>
    bool UConfigurable<T>::Validate( const json& source )
    {
        json patch = json::diff( source, fDefaultConfig ); 
        for( auto it = patch.begin(); it != patch.end(); ++it )
        {
            // key "op" MUST be in any patch according to
            // https://datatracker.ietf.org/doc/html/rfc6902
            // and its value MUST be one of the following
            // "add", "remove", "replace", "move", "copy", or "test"
            if( it->at("op") == "replace" )
            {
                // if "op" is "replace" then there MUST be the "value" key
                if( not (it->at("value").is_null()) )
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    template<typename T>
    void UConfigurable<T>::ReadConfig( json &config )
    {
        config = this->fDefaultConfig;

        if( Validate( config ) )
        {
            try
            {
                this->ReadConfigImpl( config );
            }
            catch( json::exception &e )
            {
                throw VException( VError_t::vConfigError, e.what() );
            }
        }
        else
        {
            throw VException( VError_t::vConfigError, "Config validation failed" );
        }
    }    

    template<typename T>
    void UConfigurable<T>::WriteConfig( const json &config )
    {
        if( Validate( config ) )
        {
            try
            {
                this->WriteConfigImpl( config );
            }
            catch( json::exception &e )
            {
                throw VException( VError_t::vConfigError, e.what() );
            }
        }
        else
        {
            throw VException( VError_t::vConfigError, "Config validation failed" );
        }
    }    


    void WriteConfigToFile( const json& j, const std::string& path );

    json ReadConfigFromFile( const std::string& path );
}
#endif
