#include <nlohmann/json.hpp>

class UConfigurable
{
    public :
        UConfigurable() {};
        virtual ~UConfigurable() {};

        virtual void ReadConfig( nlohmann::json &config ) = 0;
        virtual void WriteConfig( const nlohmann::json &config ) = 0;
};
