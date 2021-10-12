#include "VException.h"
#include "modules/V1785N.h"

#include <iostream>
#include <cstring>
#include <iomanip>

namespace vmepp
{

    UEvent<V1785N>::UEvent() :
        fHeader( V1785N_WORD_TYPE_HEADER ),//Default header should be here
        fData{ 0 },
        fEOB( V1785N_WORD_TYPE_EOB )//Default eob should be here
    {
    };

    UEvent<V1785N>::UEvent( const UEvent<V1785N> &other ) :
        fHeader( other.fHeader ),
        fEOB( other.fEOB )
    {
        std::memcpy( fData, other.fData, sizeof( other.fData ) );
    }

    UEvent<V1785N>& UEvent<V1785N>::operator=( UEvent<V1785N> other )
    {
        swap( *this, other );
        return *this;
    }

    //****** V1785N Part ******
    V1785N::V1785N( uint32_t baseAddress, uint32_t range ) :
        VSlave( baseAddress, range ),
        VSlaveAcquisitor( baseAddress, range ),
        VSlaveInterrupter( baseAddress, range ),

        fFirmwareRevision( "N/A" ),
        fOUI( 0 ),
        fVersion( 0 ),
        fBoardID( 0 ),
        fRevision( 0 )
    {
    }

    template<>
    const std::string UConfigurable<V1785N>::fName = "V1785N";

    void V1785N::Initialize()
    {
        PrintMessage( Message_t::INFO, "Inititalizing " + fName + "..." );

        //READ ROM
        PrintMessage( Message_t::INFO, "Reading ROM of " + fName + "..." );
        ReadSerialNumber();
        ReadFirmRevNumber();
        ReadOUI();
        ReadVersion();
        ReadBoardID();
        ReadRevision();

        Reset();

        PrintMessage( Message_t::INFO, "Inititalizing " + fName + "...OK" );
        return;
    }

    void V1785N::SetBit16( uint32_t address, uint16_t bit )
    {
        //This function is overriding the base class function in some cases
        switch( address )
        {
            case( V1785N_BIT_CLEAR_1 ) :
                WriteRegister16( V1785N_BIT_CLEAR_1, (1U << bit) );
                break;
            case( V1785N_BIT_CLEAR_2 ) :
                WriteRegister16( V1785N_BIT_CLEAR_2, (1U << bit) );
                break;
            default :
                //Use the base class function
                VSlave::SetBit16( address, bit );
                break;
        }
    }

    void V1785N::ClearBit16( uint32_t address, uint16_t bit )
    {
        //This function is overriding the base class function in some cases
        switch( address )
        {
            case( V1785N_BIT_SET_1 ) :
                SetBit16( V1785N_BIT_CLEAR_1, bit );
                break;
            case( V1785N_BIT_SET_2 ) :
                SetBit16( V1785N_BIT_CLEAR_2, bit );
                break;
            default :
                //Use the base class function
                VSlave::ClearBit16( address, bit );
                break;
        }
    }

    //****** READING ROM + ******
    uint16_t V1785N::ReadVersion()
    {
        //Data seems to be a 1-byte long
        fVersion = ReadRegister16( V1785N_VERSION, 0x00FFU );
        return fVersion;
    }

    uint32_t V1785N::ReadBoardID()
    {
        //Data seems to be a 1-byte long
        uint16_t msb = ReadRegister16( V1785N_BOARD_ID_MSB, 0x00FF );
        uint16_t middle = ReadRegister16( V1785N_BOARD_ID, 0x00FF );
        uint16_t lsb = ReadRegister16( V1785N_BOARD_ID_LSB, 0x00FF );

        fBoardID = (lsb & 0xFFUL) | ((middle & 0xFFUL) << 8U) | ((msb & 0xFFUL) << 16);

        return fBoardID;
    }

    uint32_t V1785N::ReadOUI()
    {
        //Data seems to be a 1-byte long
        uint16_t msb    = ReadRegister16( V1785N_OUI_MSB, 0x00FFU );
        uint16_t middle = ReadRegister16( V1785N_OUI, 0x00FFU );
        uint16_t lsb    = ReadRegister16( V1785N_OUI_LSB, 0x00FFU );
        fOUI = (lsb) | (middle << 8U) | (msb << 16U);
        return fOUI;
    }

    uint16_t V1785N::ReadRevision()
    {
        //Data seems to be a 1-byte long
        fRevision = ReadRegister16( V1785N_REVISION, 0x00FFU );
        return fRevision;
    }

    uint16_t V1785N::ReadSerialMSB()
    {
        return ReadRegister16( V1785N_SERIAL_MSB );
    }

    uint16_t V1785N::ReadSerialLSB()
    {
        return ReadRegister16( V1785N_SERIAL_LSB );
    }

    uint16_t V1785N::ReadSerialNumber()
    {
        fSerial = (ReadSerialLSB() & 0x00FFU) | (ReadSerialMSB() << 8U);
        return fSerial;
    }

    uint16_t V1785N::ReadFirmRevNumber()
    {
        uint16_t data = ReadRegister16( V1785N_FIRMWARE_REVISION );
        fFirmwareRevision =  std::to_string( (data & 0xF000U) >> 12U ) +
                             std::to_string( (data & 0x0F00U) >> 8U ) + "." +
                             std::to_string( (data & 0x00F0U) >> 4 ) +
                             std::to_string( (data & 0x000FU) );
        fFirmware = fFirmwareRevision;
        return data;
    }
    //****** READING ROM - ******

    //****** PRINTTING + ******
    void V1785N::Print() const
    {
        std::cout << std::right << "\n";
        std::cout << std::right << "     /" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "/-\n";
        std::cout << std::right << "    /" << std::setfill(' ') << std::setw(60) << " " <<                                                                       "/-|\n";
        std::cout << std::right << "   /" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "/- |\n";
        std::cout << std::setfill(' ');
        std::cout << std::right << "   |" << std::setw(30) << fName << std::setw(30) << " " <<                                                                  "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|-_|\n";
        std::cout << std::right << "   |" << std::setw(30) << "FW Revision : " << std::left << std::setw(30) << GetFirmRevision() <<                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|  |\n";
        std::cout << std::right << "   |" << std::setw(30) << "OUI : " << std::left << std::setw(30) << GetOUI() <<                                             "| /-\n";
        std::cout << std::right << "   |" << std::setw(30) << "Version : " << std::left << std::setw(30) << GetVersion() <<                                     "|/-|\n";
        std::cout << std::right << "   |" << std::setw(30) << "Board ID : " << std::left << std::setw(30) << GetBoardID() <<                                    "|- |\n";
        std::cout << std::right << "   |" << std::setw(30) << "Revision : " << std::left << std::setw(30) << GetRevision() <<                                   "|- |\n";
        std::cout << std::right << "   |" << std::setw(30) << "Serial Number : " << std::left << std::setw(30) << GetSerialNumber() <<                          "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- |\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|- _\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|-_/\n";
        std::cout << std::right << "   |" << std::setw(60) << " " <<                                                                                            "|-/ \n";
        std::cout << std::right << "   |" << std::setfill('=') << std::setw(60) << "=" <<                                                                       "|/\n";
        std::cout << std::setfill(' ');
        std::cout << "\n";
    }

    void V1785N::PrintControl()
    {
        uint16_t data = ReadRegister16( V1785N_CONTROL_REGISTER_1 );
        std::cout << "\n" << "CONTROL 1:" << "\n";
        std::cout << std::setw(20) << "BLKEND : "       << std::setw(10) << bool(data & (1U << V1785N_CNTL_REG_BLKEND_BIT))     << "\n";
        std::cout << std::setw(20) << "PROG RESET : "   << std::setw(10) << bool(data & (1U << V1785N_CNTL_REG_PRG_RST_BIT))    << "\n";
        std::cout << std::setw(20) << "BERR ENABLE : "  << std::setw(10) << bool(data & (1U << V1785N_CNTL_REG_BERR_ENB_BIT))   << "\n";
        std::cout << std::setw(20) << "ALIGN64 : "      << std::setw(10) << bool(data & (1U << V1785N_CNTL_REG_ALN64_BIT))      << "\n\n";
    }

    void V1785N::PrintBitSet()
    {
        uint16_t data = ReadRegister16( V1785N_BIT_SET_1 );
        std::cout << "\n" << "BIT SET 1:" << "\n";
        std::cout << std::setw(20) << "BERR FLAG : "    << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_1_BERR_FLAG_BIT)) << "\n";
        std::cout << std::setw(20) << "SEL ADDR : "     << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_1_SEL_ADDR_BIT))  << "\n";
        std::cout << std::setw(20) << "SOFT RESET : "   << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_1_SOFT_RST_BIT))  << "\n";

        data = ReadRegister16( V1785N_BIT_SET_2 );
        std::cout << "BIT SET 2:" << "\n";
        std::cout << std::setw(20) << "MEM TEST : "         << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_MEM_TEST_BIT)) << "\n";
        std::cout << std::setw(20) << "OFFLINE : "          << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_OFFLINE_BIT)) << "\n";
        std::cout << std::setw(20) << "CLEAR DATA : "       << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_CLR_DATA_BIT)) << "\n";
        std::cout << std::setw(20) << "OVER RANGE PROG: "   << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_OV_RNG_PRG_BIT)) << "\n";
        std::cout << std::setw(20) << "LOW THRESH PROG : "  << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_LW_THR_PRG_BIT)) << "\n";
        std::cout << std::setw(20) << "TEST ACQ : "         << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_TEST_ACQ_BIT)) << "\n";
        std::cout << std::setw(20) << "SLD ENABLE : "       << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_SLD_ENB_BIT)) << "\n";
        std::cout << std::setw(20) << "STEP TH : "          << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_STEP_TH_BIT)) << "\n";
        std::cout << std::setw(20) << "AUTO INCR : "        << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_AUTO_INC_BIT)) << "\n";
        std::cout << std::setw(20) << "EMPTY PROG : "       << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_EMPTY_PRG_BIT)) << "\n";
        std::cout << std::setw(20) << "SLD SUB ENABLE : "   << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_SLD_SB_ENB_BIT)) << "\n";
        std::cout << std::setw(20) << "ALL TRG : "          << std::setw(10) << bool(data & (1U << V1785N_BIT_SET_2_ALL_TRG_BIT)) << "\n\n";
    }

    void V1785N::PrintStatus()
    {
        uint16_t data = ReadRegister16( V1785N_STATUS_REGISTER_1 );
        std::cout << "\n" << "STATUS 1:" << "\n";
        std::cout << std::setw(20) << "DREADY : "       << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_DREADY_BIT)) << "\n";
        std::cout << std::setw(20) << "GLB DREADY : "   << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_GLB_DREADY_BIT)) << "\n";
        std::cout << std::setw(20) << "BUSY : "         << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_BUSY_BIT)) << "\n";
        std::cout << std::setw(20) << "GLB BUSY : "     << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_GLB_BUSY_BIT)) << "\n";
        std::cout << std::setw(20) << "AMNESIA : "      << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_AMNESIA_BIT)) << "\n";
        std::cout << std::setw(20) << "PURGED : "       << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_PURGED_BIT)) << "\n";
        std::cout << std::setw(20) << "TERM ON : "      << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_TERM_ON_BIT)) << "\n";
        std::cout << std::setw(20) << "TERM OFF : "     << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_TERM_OFF_BIT)) << "\n";
        std::cout << std::setw(20) << "EVRDY : "        << std::setw(10) << bool(data & (1U << V1785N_STS1_REG_EVRDY_BIT)) << "\n";

        data = ReadRegister16( V1785N_STATUS_REGISTER_2 );
        std::cout << "STATUS 2:" << "\n";
        std::cout << std::setw(20) << "BUFF EMPTY : "   << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_BUFF_EMPTY_BIT)) << "\n";
        std::cout << std::setw(20) << "BUFF FULL : "    << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_BUFF_FULL_BIT)) << "\n";
        std::cout << std::setw(20) << "DSEL 0 : "       << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_DSEL0_BIT)) << "\n";
        std::cout << std::setw(20) << "DSEL 1 : "       << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_DSEL1_BIT)) << "\n";
        std::cout << std::setw(20) << "CSEL 0 : "       << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_CSEL0_BIT)) << "\n";
        std::cout << std::setw(20) << "CSEL 1 : "       << std::setw(10) << bool(data & (1U << V1785N_STS2_REG_CSEL1_BIT)) << "\n\n";
    }
    //****** PRINTTING - ******

    //****** CHANNELS + ******
    void V1785N::EnableAll( bool alive )
    {
        for( uint16_t ch = 0; ch < V1785N_N_CHANNELS; ch++ )
        {
            if( alive )
            {
                ClearBit16( V1785N_HIGH_THRESHOLD(ch), V1785N_THR_KILL_BIT );
                ClearBit16( V1785N_LOW_THRESHOLD(ch), V1785N_THR_KILL_BIT );
            }
            else
            {
                SetBit16( V1785N_HIGH_THRESHOLD(ch), V1785N_THR_KILL_BIT );
                SetBit16( V1785N_LOW_THRESHOLD(ch), V1785N_THR_KILL_BIT );
            }
        }
    }

    void V1785N::WriteLowThreshold( uint16_t ch, uint16_t thr, bool kill )
    {
        if( kill ) { thr |= (1U << V1785N_THR_KILL_BIT); }
        else       { thr &= ~(1U << V1785N_THR_KILL_BIT); }
        WriteRegister16( V1785N_LOW_THRESHOLD(ch), thr );
    }

    void V1785N::WriteHighThreshold( uint16_t ch, uint16_t thr, bool kill )
    {
        if( kill ) { thr |= (1U << V1785N_THR_KILL_BIT); }
        else       { thr &= ~(1U << V1785N_THR_KILL_BIT); }
        WriteRegister16( V1785N_HIGH_THRESHOLD(ch), thr );
    }

    uint16_t V1785N::ReadLowThreshold( uint16_t ch )
    {
        return ReadRegister16( V1785N_LOW_THRESHOLD(ch), V1785N_THRESHOLD_MSK );
    }

    uint16_t V1785N::ReadHighThreshold( uint16_t ch )
    {
        return ReadRegister16( V1785N_HIGH_THRESHOLD(ch), V1785N_THRESHOLD_MSK );
    }

    bool V1785N::ReadIfEnabledLow( uint16_t ch )
    {
        return not ReadRegister16( V1785N_LOW_THRESHOLD(ch), V1785N_THR_KILL_MSK );
    }

    bool V1785N::ReadIfEnabledHigh( uint16_t ch )
    {
        return not ReadRegister16( V1785N_HIGH_THRESHOLD(ch), V1785N_THR_KILL_MSK );
    }
    //****** CHANNELS - ******

    //****** MISC + ******
    void V1785N::Reset()
    {
        PrintMessage( Message_t::INFO, "Resetting " + fName + "...\n" );

        //Crate number 0
        WriteRegister16( V1785N_CRATE_SELECT, 0x0000 );

        PrintMessage( Message_t::INFO, "Setting the registers of " + fName + "..." );

        //Bit Set 1
        ClearBit16( V1785N_BIT_SET_1, V1785N_BIT_SET_1_BERR_FLAG_BIT );
        ClearBit16( V1785N_BIT_SET_1, V1785N_BIT_SET_1_SEL_ADDR_BIT );

        //Control
        ClearBit16( V1785N_CONTROL_REGISTER_1, V1785N_CNTL_REG_BLKEND_BIT );
        SetBit16( V1785N_CONTROL_REGISTER_1, V1785N_CNTL_REG_ALN64_BIT );
        ClearBit16( V1785N_CONTROL_REGISTER_1, V1785N_CNTL_REG_BERR_ENB_BIT );

        //Sliding scale is enabled
        SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_SLD_ENB_BIT );
        //Read pointer incremented automatically
        SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_AUTO_INC_BIT );
        //Zero Suppression: ADC < 2 * Threshold
        SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_STEP_TH_BIT );

        //Event counter incremented only on accepted events
        ClearBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_ALL_TRG_BIT );

        //ClearData();

        PrintMessage( Message_t::INFO, "Resetting " + fName + "...OK\n" );
    }

    void V1785N::SendSingleShotReset()
    {
        uint16_t data = 1;
        WriteRegister16( V1785N_SINGLE_SHOT_RST, data );
    }

    void V1785N::SendSoftReset()
    {
        SetBit16( V1785N_BIT_SET_1, V1785N_BIT_SET_1_SOFT_RST_BIT );
        ClearBit16( V1785N_BIT_SET_1, V1785N_BIT_SET_1_SOFT_RST_BIT );
    }
    //****** MISC - ******

    //****** INTERRUPTS + ******
    void V1785N::WriteIRQEvents( uint16_t n )
    {
        WriteRegister16( V1785N_EVENT_TRIGGER_REGISTER, n, V1785N_EVENT_TRIGGER_REG_MSK );
    }

    uint16_t V1785N::ReadIRQEvents()
    {
        return ReadRegister16( V1785N_EVENT_TRIGGER_REGISTER, V1785N_EVENT_TRIGGER_REG_MSK );
    }

    void V1785N::WriteIRQLevel( uint16_t level )
    {
        WriteRegister16( V1785N_INTERRUPT_LEVEL, level, V1785N_INTERRUPT_LEVEL_MSK );
    }

    uint16_t V1785N::ReadIRQLevel()
    {
        return ReadRegister16( V1785N_INTERRUPT_LEVEL, V1785N_INTERRUPT_LEVEL_MSK );
    }

    void V1785N::WriteIRQVector( uint16_t vector )
    {
        WriteRegister16( V1785N_INTERRUPT_VECTOR, vector, V1785N_INTERRUPT_VECTOR_MSK );
    }

    uint16_t V1785N::ReadIRQVector()
    {
        return ReadRegister16( V1785N_INTERRUPT_VECTOR, V1785N_INTERRUPT_VECTOR_MSK );
    }

    void V1785N::ISR( uint16_t vector )
    {
        PrintMessage( Message_t::INFO, "V1785N ISR" );
        return;
    }
    //****** INTERRUPTS - ******

    //****** DATA ACQUISITION + ******
    void V1785N::AllocateBuffer()
    {
        ResetIndex();
        if( fBuffer )
        {
            PrintMessage( Message_t::WARNING, "Trying to reallocate buffer (not nullptr)" );
        }
        try
        {
            fBuffer.reset( new uint32_t[V1785N_MAX_MBLT_SIZE / 4] );
        }
        catch( std::bad_alloc &e )
        {
            fBuffer.reset();
            throw VException( VError_t::vBuffAllocFailed, "from V1785N::AllocateBuffer" );
        }
    }


    uint32_t V1785N::ReadBuffer()
    {
        ResetIndex();
        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "Read buffer : buffer is nullptr. Forgot to allocate?" );
            return 0;
        }
        int count;
        MBLTReadRequest( V1785N_OUTPUT_BUFFER_START, fBuffer.get(), V1785N_MAX_MBLT_SIZE, &count );
        return (fReadBytes = (count > 0) ? count : 0);
    }

    bool V1785N::GetEventAt( uint32_t index, UEvent<V1785N> &event ) const
    {
        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "GetEventAt : buffer is nullptr. Forgot to allocate?" );
            return false;
        }

        //Skip invalid data if present
        for( ; index < fReadBytes / 4; index++ )
        {
            if( (fBuffer[index] & V1785N_WORD_TYPE_MSK) != V1785N_WORD_TYPE_INVALID )
            {
                break;
            }
        }

        uint32_t word;
        uint16_t chCurrent;
        uint32_t wordTypeCurrent = V1785N_WORD_TYPE_INVALID;
        uint32_t wordTypeExpected = V1785N_WORD_TYPE_HEADER;

        for( ; index < fReadBytes / 4; index++ )
        {
            word = fBuffer[index];
            wordTypeCurrent = word & V1785N_WORD_TYPE_MSK;
            if( wordTypeCurrent != wordTypeExpected )
            {
                return false;
            }

            switch( wordTypeCurrent )
            {
                case( V1785N_WORD_TYPE_HEADER ):
                    chCurrent = 0;
                    event.fHeader = word;
                    event.fStart = index;
                    wordTypeExpected = event.GetMemoChannels() ? V1785N_WORD_TYPE_DATA : V1785N_WORD_TYPE_EOB;
                    break;
                case( V1785N_WORD_TYPE_DATA ) :
                    event.fData[chCurrent] = word;
                    chCurrent++;
                    wordTypeExpected = (chCurrent < event.GetMemoChannels()) ? V1785N_WORD_TYPE_DATA : V1785N_WORD_TYPE_EOB;
                    break;
                case( V1785N_WORD_TYPE_EOB ):
                    event.fEOB = word;
                    event.fStop = index;
                    return true;
                case( V1785N_WORD_TYPE_INVALID ):
                default:
                    return false;
            }
        }
        return false;
    }


    void V1785N::ClearData()
    {
        SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_CLR_DATA_BIT );
        ClearBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_CLR_DATA_BIT );
    }

    void V1785N::EnableZeroSupp( bool status )
    {
        if( status )
        {
            ClearBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_LW_THR_PRG_BIT );
        }
        else
        {
            SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_LW_THR_PRG_BIT );
        }
    }

    void V1785N::EnableOverSupp( bool status )
    {
        if( status )
        {
            ClearBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_OV_RNG_PRG_BIT );
        }
        else
        {
            SetBit16( V1785N_BIT_SET_2, V1785N_BIT_SET_2_OV_RNG_PRG_BIT );
        }
    }
    //****** DATA ACQUISITION - ******
    //
    void V1785N::ReadConfig( UConfig<V1785N>& config )
    {
    }

    void V1785N::WriteConfig( const UConfig<V1785N>& config )
    {
    }
}
