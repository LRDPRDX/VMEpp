#include <iostream>
#include <fstream>
#include <algorithm>

#include <CAENDigitizer.h>

#include "modules/V1742B.h"

CAEN_DGTZ_DRS4Correction_t cTable[MAX_X742_GROUP_SIZE];

int main( int argc, char **argv )
{
    int handle = 0;
    uint32_t address = 0x50000000; 

    CAEN_DGTZ_ErrorCode e = CAEN_DGTZ_OpenDigitizer( CAEN_DGTZ_OpticalLink, 0, 0, address, &handle );
    if( e )
    {
        std::cout << "ERROR :: Opening digitizer\n";
        return -1;
    }
    e = CAEN_DGTZ_LoadDRS4CorrectionData( handle, CAEN_DGTZ_DRS4_1GHz );
    if( e )
    {
        std::cout << "ERROR :: Loading corrections\n";
        return -1;
    }
    e = CAEN_DGTZ_GetCorrectionTables( handle, CAEN_DGTZ_DRS4_1GHz, cTable );
    if( e )
    {
        std::cout << "ERROR :: Reading corrections\n";
        return -1;
    }

    vmepp::V1742B::CorrectionTable table;
    for( uint8_t g = 0; g < vmepp::V1742B::GetGroupNumber(); ++g )
    {
        uint8_t ch;
        //FIll Channels
        for( ch = 0; ch < table.table[g].channels.size(); ++ch )
        {
            std::copy( cTable[g].cell[ch], cTable[g].cell[ch] + table.table[g].channels[ch].cell.size(),
                       table.table[g].channels[ch].cell.begin() );
            std::copy( cTable[g].nsample[ch], cTable[g].nsample[ch] + table.table[g].channels[ch].sample.size(),
                       table.table[g].channels[ch].sample.begin() );
        }
        //Fill TRn
        std::copy( cTable[g].cell[ch], cTable[g].cell[ch] + table.table[g].trN.cell.size(),
                   table.table[g].trN.cell.begin() );
        std::copy( cTable[g].nsample[ch], cTable[g].nsample[ch] + table.table[g].trN.sample.size(),
                   table.table[g].trN.sample.begin() );
        //Fill Time
        std::copy( cTable[g].time, cTable[g].time + table.table[g].time.size(),
                   table.table[g].time.begin() );
    }

    std::ofstream fo( "correction.x742_corr", std::ios_base::trunc | std::ios_base::out | std::ios_base::binary );
    fo << table;

    std::ifstream fi( "correction.x742_corr", std::ios_base::in | std::ios_base::binary );
    fi >> table;

    e = CAEN_DGTZ_CloseDigitizer( handle );
    if( e )
    {
        std::cout << "ERROR :: Closing digitizer\n";
        return -1;
    }

    std::cout << "! SUCCESS !\n";

    return 0;
}
