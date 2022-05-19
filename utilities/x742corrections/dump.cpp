#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include <CAENDigitizer.h>

#include "modules/V1742B.h"

using namespace vmepp;

CAEN_DGTZ_DRS4Correction_t cTable[MAX_X742_GROUP_SIZE];

void DumpTable( int handle, std::string fileName )
{
    fileName += ".x742_corr";
    std::ofstream fo( fileName, std::ios_base::trunc | std::ios_base::out | std::ios_base::binary );
    PrintMessage( Message_t::INFO, "Writing correction tables to " + fileName + " ..." );

    for( auto freq : V1742B::RateIterator() )
    {
        auto caenFreq = static_cast<CAEN_DGTZ_DRS4Frequency_t>( freq );
        CAEN_DGTZ_ErrorCode e = CAEN_DGTZ_LoadDRS4CorrectionData( handle, caenFreq );
        if( e )
        {
            PrintMessage( Message_t::ERROR, "Loading corrections from the board\n" );
            return;
        }
        e = CAEN_DGTZ_GetCorrectionTables( handle, caenFreq, cTable );
        if( e )
        {
            PrintMessage( Message_t::ERROR, "Getting corrections from the board\n" );
            return;
        }

        V1742B::FreqCorrection table;
        table.freq = freq;
        for( uint8_t g = 0; g < V1742B::GetGroupNumber(); ++g )
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

        fo << table;
    }
    if( fo.good() )
    {
        PrintMessage( Message_t::INFO, "Writing corrections SUCCEEDED !" );
    }
    else
    {
        PrintMessage( Message_t::ERROR, "Writing corrections FAILED !" );
    }
}

int main( int argc, char **argv )
{
    int handle = 0;
    uint32_t address = 0x50000000;

    CAEN_DGTZ_ErrorCode e = CAEN_DGTZ_OpenDigitizer( CAEN_DGTZ_OpticalLink, 0, 0, address, &handle );
    if( e )
    {
        PrintMessage( Message_t::ERROR, "Opening digitizer FAILED !" );
        return -1;
    }

    std::string fileName = "default";
    if( argc > 1 ){ fileName = argv[1]; }

    DumpTable( handle, fileName );

    e = CAEN_DGTZ_CloseDigitizer( handle );
    if( e )
    {
        PrintMessage( Message_t::ERROR, "Closing digitizer FAILED !" );
        return -1;
    }

    return 0;
}
