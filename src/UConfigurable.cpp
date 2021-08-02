#include <fstream>
#include "UConfigurable.h"

namespace vmeplus
{

    void WriteConfigToFile( const json& j, const std::string& path )
    {
        std::ofstream file;
        file.exceptions( std::ios::failbit | std::ios::badbit );

        try
        {
            file.open( path );
            std::string s = j.dump( 2 );
            file << s << std::endl;
            file.close();
        }
        catch( std::ofstream::failure& e )
        {
            PrintMessage( Message_t::ERROR, e.what() );
        }
    }

    json ReadConfigFromFile( const std::string& path )
    {
        std::ifstream file;
        file.exceptions( std::ios::failbit | std::ios::badbit );
        json j;

        try
        {
            file.open( path );
            j = json::parse( file );
            file.close();
        }
        catch( std::ifstream::failure& e )
        {
            PrintMessage( Message_t::ERROR, e.what() );
        }
        return j;
    }  
}
