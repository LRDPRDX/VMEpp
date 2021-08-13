#ifndef V_PLUS_CONFIGURABLE_H
#define V_PLUS_CONFIGURABLE_H

#include <nlohmann/json.hpp>


namespace vmeplus
{
    using json = nlohmann::json;

    // Curiously recurring template pattern
    template <typename T>
    class UConfigurable
    {
        protected :
            static json   fDefaultConfig;

        public :
            UConfigurable() {};
            virtual ~UConfigurable() {};

            virtual void ReadConfig( nlohmann::json &config ) = 0;
            virtual void WriteConfig( const nlohmann::json &config ) = 0;

            static json GetDefaultConfig() { return fDefaultConfig; }
    };

    void WriteConfigToFile( const json& j, const std::string& path );

    json ReadConfigFromFile( const std::string& path );
}
#endif
