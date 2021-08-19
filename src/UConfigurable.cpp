#include <fstream>
#include "UConfigurable.h"

namespace vmeplus
{
    void WriteConfigToFile( const json& j, const std::string& path )
    {
        std::ofstream file;
        file.exceptions( std::ios::failbit | std::ios::badbit );

        file.open( path );
        std::string s = j.dump( 2 );
        file << s << std::endl;
        file.close();
    }

    json ReadConfigFromFile( const std::string& path )
    {
        try
        {
            std::ifstream file;
            file.exceptions( std::ios::failbit | std::ios::badbit );
            file.open( path );

            json j = json::parse( file );

            file.close();

            return j;
        }
        catch( const json::exception& e )
        {
            throw( VException( VError_t::vConfigError, e.what() ) );
        }
    }
}
