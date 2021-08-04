#ifndef V_PLUS_UCONFIGURABLE_H
#define V_PLUS_UCONFIGURABLE_H

#include <nlohmann/json.hpp>


namespace vmeplus
{
    // Curiously recurring template pattern
    template <typename T>
    class UConfigurable
    {
        protected :
            static nlohmann::json   fDefaultConfig;
    
        public :
            UConfigurable() {};
            virtual ~UConfigurable() {};
    
            virtual void ReadConfig( nlohmann::json &config ) = 0;
            virtual void WriteConfig( const nlohmann::json &config ) = 0;
    
            static nlohmann::json GetDefaultConfig() { return fDefaultConfig; }
    };
}
#endif
