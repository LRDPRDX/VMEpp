#include <fstream>
#include <iostream>

#include "CAENVMElib.h"
#include "modules/V1190B.h"

namespace vmeplus {

    /*************************/
    /****** V1190BEvent ******/
    /*************************/
    V1190BEvent::V1190BEvent()
    {
    }

    V1190BEvent::~V1190BEvent()
    {
    }

    /********************/
    /****** V1190B ******/
    /********************/
    V1190B::V1190B(uint32_t baseAddress, uint32_t range) :
            VSlave("V1190B", baseAddress, range),
            VSlaveInterrupter("V1190B", baseAddress, range),
            VSlaveAcquisitor("V1190B", baseAddress, range),

            fFirmwareRevision("N/A"),
            fOUI(0),
            fVersion(0),
            fBoardID(0),
            fRevision(0) {}

    V1190B::~V1190B() {};

    void V1190B::Initialize() {
        PrintMessage(Message_t::INFO, "Inititalizing " + fName + "...");

        //READ ROM
        PrintMessage(Message_t::INFO, "\tReading ROM of " + fName + "...");
        ReadSerialNumber();
        ReadFirmRevNumber();
        ReadOUI();
        ReadVersion();
        ReadBoardID();
        ReadRevision();
        PrintMessage(Message_t::INFO, "\tReading ROM of " + fName + "...OK");

        //Control register
        PrintMessage(Message_t::INFO, "\tConfiguration of the Control register of " + fName + "...");
        WriteControl( Control_t::BERR_EN, false );
        WriteControl( Control_t::TERM_SW );
        WriteControl( Control_t::TERM );
        WriteControl( Control_t::EMPTY_EVENT, false );
        WriteControl( Control_t::ALIGN64 );
        WriteControl( Control_t::COMPENSATION_EN );
        WriteControl( Control_t::TEST_FIFO_EN, false );
        WriteControl( Control_t::READ_COMPENSATION );
        WriteControl( Control_t::EVENT_FIFO_EN );
        WriteControl( Control_t::ETTT_EN );
        WriteControl( Control_t::MB16_ADDR_EN, false );
        PrintMessage(Message_t::INFO, "\tConfiguration of the Control register of " + fName + "...OK");

        WriteEnableHeaderTrailer();

        WriteIRQLevel( 0 ); // No Interrupt

        WriteSoftwareClear(); // Clear Buffers

        PrintMessage(Message_t::INFO, "Inititalizing " + fName + "...OK");
    }

    void V1190B::Release() {}

    /****** DATA ACQUISITION ******/
    V1190B::EventFIFO V1190B::ReadEventFIFO()
    {
        uint32_t data = ReadRegister32( V1190B_EVENT_FIFO );
        EventFIFO ef;
        ef.eventCount = (data >> 16U) & 0x00FF;
        ef.wordCount = (data & 0x00FF);
        return ef;
    }

    uint16_t V1190B::ReadEventFIFOStored()
    {
        return ReadRegister16( V1190B_EVENT_FIFO_STORED );
    }

    void V1190B::WriteTestreg( uint32_t data )
    {
        WriteRegister32( V1190B_TESTREG, data );
    }

    uint32_t V1190B::ReadTestreg()
    {
        return ReadRegister32( V1190B_TESTREG );
    }

    void V1190B::WriteEventCountReset()
    {
        WriteRegister16( V1190B_SOFTWARE_EVENT_RESET, 1 );
    }

    void V1190B::WriteEventsBLT(uint8_t n) {
        WriteRegister16(V1190B_BLT_EVENT_NUMBER, n, 0x00FF);
    }

    uint8_t V1190B::ReadEventsBLT() {
        return ReadRegister16(V1190B_BLT_EVENT_NUMBER, 0x00FF);
    }

    uint32_t V1190B::ReadEventCounter() {
        return ReadRegister32(V1190B_EVENT_COUNTER);
    }

    uint16_t V1190B::ReadEventsStored() {
        return ReadRegister16(V1190B_EVENT_STORED);
    }

    void V1190B::SendSWTrigger() {
        WriteRegister16(V1190B_SOFTWARE_TRIGGER, 1);
    }

    /****** DATA ACQUISITION ******/
    void V1190B::AllocateBuffer()
    {
        ResetIndex();
        if( fBuffer )
        {
            PrintMessage( Message_t::WARNING, "Trying to reallocate buffer (not nullptr)" );
        }
        try
        {
            fBuffer.reset( new uint32_t[2048 / 4] );
        }
        catch( std::bad_alloc &e )
        {
            fBuffer.reset(); //noexcept
            throw VException( VError_t::vBuffAllocFailed, "from V1190B::AllocateBuffer" );
        }
    }

    uint32_t V1190B::ReadBuffer()
    {
        ResetIndex();

        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "V1190B::ReadBuffer() : buffer is nullptr. Forgot to allocate?" );
            return 0;
        }
        int count;
        MBLTReadRequest( V1190B_OUTPUT_BUFFER, fBuffer.get(), 2048, &count );
        return (fReadBytes = (count > 0) ? count : 0);
    }

    void V1190B::DropBuffer( const std::string& fileName )
    {
        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "V1190B::DropBuffer() : buffer is nullptr. Forgot to allocate?" );
            return;
        }

        std::ofstream file;
        file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {
            file.open( fileName, std::ios::binary | std::ios::trunc );
            file.write((char*)fBuffer.get(), fReadBytes);
        }
        catch( const std::ofstream::failure& e )
        {
            PrintMessage( Message_t::WARNING, "Couldn't write to the file \"" + fileName + "\"." );
        }
    }

    bool V1190B::GetEventAt(uint32_t index, VEvent *e) const
    {
        V1190BEvent* event = dynamic_cast<V1190BEvent*>( e ); 
        if( !event )
        {
            PrintMessage( Message_t::ERROR, "Bad event pointer has been provided to the V1190B::GetEventAt() function. Must be V1190BEvent*." );
            return false;
        }

        if( !fBuffer )
        {
            PrintMessage( Message_t::WARNING, "V1190B::GetEventAt : buffer is nullptr. Forgot to allocate?" );
            return false;
        }

        event->fHits.clear();

        uint32_t word;

        // Skip fillers if present
        for( ; index < fReadBytes / 4; ++index )
        {
            word = fBuffer[index];
            if( (word & V1190B::Word_t::MASK) != V1190B::Word_t::FILLER )
            {
                break;
            }
        }

        uint32_t wordTypeCurrent = word & V1190B::Word_t::MASK;

        // If the first non-filler word is T_MEAS it means we are
        // collecting data recorded in the CONTINUOUS mode
        if( wordTypeCurrent == V1190B::Word_t::T_MEAS )
        {
            event->fHits.push_back( V1190BEvent::V1190BHit( word ) );
            event->fStart = event->fStop = index;
            return true;
        }

        //uint32_t wordTypeExpected = V1190B::Word_t::G_HEADER;

        //for( ; index < fReadBytes / 4; ++index )
        //{
        //    word = fBuffer[index];
        //    wordTypeCurrent = word & V1190B::Word_t::MASK;

        //    switch( wordTypeCurrent )
        //    {
        //        case( V1190B::Word_t::T_MEAS ) :
        //            event->fMeasurements.push_back( word );
        //            break;
        //    }
        //}

        return false;
    }

    /****** INTERRUPTER ******/
    void V1190B::WriteIRQEvents(uint16_t n) {
        WriteRegister16(V1190B_ALMOST_FULL_LEVEL, (n > 0) ? n : 1);
    }

    uint16_t V1190B::ReadIRQEvents() {
        return ReadRegister16(V1190B_ALMOST_FULL_LEVEL);
    }

    void V1190B::WriteIRQLevel(uint16_t level) {
        WriteRegister16(V1190B_INTERRUPT_LEVEL, level, V1190B_INTERRUPT_LEVEL_MSK);
    }

    uint16_t V1190B::ReadIRQLevel() {
        return ReadRegister16(V1190B_INTERRUPT_LEVEL, V1190B_INTERRUPT_LEVEL_MSK);
    }

    void V1190B::WriteIRQVector(uint16_t vector) {
        WriteRegister16(V1190B_INTERRUPT_VECTOR, vector, V1190B_INTERRUPT_VECTOR_MSK);
    }

    uint16_t V1190B::ReadIRQVector() {
        return ReadRegister16(V1190B_INTERRUPT_VECTOR, V1190B_INTERRUPT_VECTOR_MSK);
    }

    void V1190B::ISR(uint16_t vector) {}

    /****** RESET ******/
    void V1190B::Reset() {
        WriteSoftwareClear();
        WriteSingleShotReset();
    }

    void V1190B::WriteSoftwareClear() {
        WriteRegister16(V1190B_SOFTWARE_CLEAR, 1);
    }

    void V1190B::WriteSingleShotReset() {
        WriteRegister16(V1190B_MODULE_RESET, 1);
    }

    void V1190B::WriteControl( V1190B::Control_t bit, bool status )
    {
        if( status ) { SetBit16( V1190B_CONTROL_REGISTER, static_cast<uint16_t>( bit ) ); }
        else         { ClearBit16( V1190B_CONTROL_REGISTER, static_cast<uint16_t>( bit ) ); }
    }

    bool V1190B::ReadControl( V1190B::Control_t bit )
    {
        return GetBit16( V1190B_CONTROL_REGISTER, static_cast<uint16_t>( bit ) ); 
    }

    uint16_t V1190B::ReadControl()
    {
        return ReadRegister16( V1190B_CONTROL_REGISTER );
    }

    bool V1190B::ReadStatus( V1190B::Status_t bit )
    {
        return GetBit16( V1190B_STATUS_REGISTER, static_cast<uint16_t>( bit ) );
    }

    uint16_t V1190B::ReadStatus()
    {
        return ReadRegister16( V1190B_STATUS_REGISTER );
    }

    /****** MICRO ******/
    void V1190B::WriteMicro(uint16_t data) {
        //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register
        // if the WO bit is set to 1, the VME can write a datum
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_WRITE_OK_MSK))) {}
        WriteRegister16(V1190B_MICRO, data);
    }

    void V1190B::WriteMicro(Opcode opcode) {
        uint16_t code = static_cast<uint16_t>(opcode.command) << 8U | opcode.object;

        //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register
        // if the WO bit is set to 1, the VME can write a datum
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_WRITE_OK_MSK))) {}
        WriteRegister16(V1190B_MICRO, code);
    }

    uint16_t V1190B::ReadMicro() {
        //a valid datum can be read via VME only if the READ_OK (RO) bit in the Micro
        //Handshake Register is set to 1
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_READ_OK_MSK))) {}
        return ReadRegister16(V1190B_MICRO);
    }

    // ACQUISITION MODE
    void V1190B::WriteAcqMode(V1190B::TriggerMode_t mode) {
        switch (mode) {
            case (TriggerMode_t::MATCHING) :
                WriteMicro(Opcode(Command::TRG_MATCH));
                break;
            case (TriggerMode_t::CONTINUOUS) :
                WriteMicro(Opcode(Command::CONT_STOR));
                break;
        }
    }

    V1190B::TriggerMode_t V1190B::ReadAcqMode() {
        WriteMicro(Opcode(Command::READ_ACQ_MOD));
        uint16_t data = ReadMicro();
        if (1U & data) return TriggerMode_t::MATCHING;     //check LSB
        else return TriggerMode_t::CONTINUOUS;
    }

    void V1190B::WriteEnableKeepToken(bool status) {
        if (status) WriteMicro(Opcode(Command::SET_KEEP_TOKEN));
        else WriteMicro(Opcode(Command::CLEAR_KEEP_TOKEN));
    }

    void V1190B::WriteLoadConfig(V1190B::Config_t config) {
        switch (config) {
            case (Config_t::DEFAULT) :
                WriteMicro(Opcode(Command::LOAD_DEF_CONFIG));
                break;
            case (Config_t::USER) :
                WriteMicro(Opcode(Command::LOAD_USER_CONFIG));
                break;
        }
    }

    void V1190B::WriteAutoLoad(V1190B::Config_t config) {
        switch (config) {
            case (Config_t::DEFAULT) :
                WriteMicro(Opcode(Command::AUTOLOAD_DEF_CONF));
                break;
            case (Config_t::USER) :
                WriteMicro(Opcode(Command::AUTOLOAD_USER_CONF));
                break;
        }
    }

    void V1190B::WriteSaveUserConfig() {
        WriteMicro(Opcode(Command::SAVE_USER_CONFIG));
    }

    // TRIGGER
    void V1190B::WriteWindowWidth(uint16_t data) {
        WriteMicro(Opcode(Command::SET_WIN_WIDTH));
        WriteMicro(data);
    }

    void V1190B::WriteWindowOffset(uint16_t data) {
        WriteMicro(Opcode(Command::SET_WIN_OFFS));
        WriteMicro(data);
    }

    void V1190B::WriteExtraSearchMargin(uint16_t data) {
        WriteMicro(Opcode(Command::SET_SW_MARGIN));
        WriteMicro(data);
    }

    void V1190B::WriteRejectMargin(uint16_t data) {
        WriteMicro(Opcode(Command::SET_REJ_MARGIN));
        WriteMicro(data);
    }

    void V1190B::WriteEnableSubTrigger(bool data) {
        if (data)
            WriteMicro(Opcode(Command::EN_SUB_TRG));
        else
            WriteMicro(Opcode(Command::DIS_SUB_TRG));
    }

    void V1190B::ReadTriggerConfiguration(TriggerData &trigger) {
        WriteMicro(Opcode(Command::READ_TRG_CONF));

        trigger.winWidth = ReadMicro();
        trigger.winOffs = ReadMicro();
        trigger.swMargin = ReadMicro();
        trigger.rejMargin = ReadMicro();
        trigger.subTrigger = ReadMicro();
    }

    // ADJUST
    void V1190B::WriteGlobalOffset(AdjustOffset adjustOffset) {
        WriteMicro(Opcode(Command::SET_GLOB_OFFS));
        WriteMicro(adjustOffset.coarseCounter & 0x0FFF);
        WriteMicro(adjustOffset.fineCounter & 0x001F);
    }

    void V1190B::ReadGlobalOffset(AdjustOffset &adjustOffset) {
        WriteMicro(Opcode(Command::READ_GLOB_OFFS));
        adjustOffset.coarseCounter = ReadMicro() & 0x0FFF;
        adjustOffset.fineCounter = ReadMicro() & 0x001F;
    }

    void V1190B::WriteAdjustChannel(uint16_t offset, uint8_t channel) {
        WriteMicro(Opcode(Command::SET_ADJUST_CH, channel % fChNumber));
        WriteMicro(offset & 0x00FF);
    }

    uint16_t V1190B::ReadAdjustChannel(uint8_t channel) {
        WriteMicro(Opcode(Command::READ_ADJUST_CH, channel % fChNumber));
        return ReadMicro() & 0x00FF;
    }

    // TDC EDGE DETECTION & RESOLUTION
    void V1190B::WriteDetection(EdgeDetect_t detect) {
        WriteMicro(Opcode(Command::SET_DETECTION));
        WriteMicro(static_cast<uint16_t>(detect) & 0x0003);
    }

    V1190B::EdgeDetect_t V1190B::ReadDetection() {
        WriteMicro(Opcode(Command::READ_DETECTION));
        return static_cast<EdgeDetect_t>(ReadMicro() & 0x0003);
    }

    void V1190B::WriteEdgeRes(TrLeadLSB lsb) {
        WriteMicro(Opcode(Command::SET_TR_LEAD_LSB));
        WriteMicro(static_cast<uint16_t>(lsb) & 0x0003);
    }

    void V1190B::WritePairRes(PairRes pairRes) {
        WriteMicro(Opcode(Command::SET_PAIR_RES));
        WriteMicro(((static_cast<uint16_t>(pairRes.width) << 8U) |
                    (static_cast<uint16_t>(pairRes.edgeTime))) & 0x0F07);
    }

    V1190B::TrLeadLSB V1190B::ReadEdgeRes() {
        WriteMicro(Opcode(Command::READ_RES));
        return static_cast<TrLeadLSB>(ReadMicro() & 0x0003);
    }

    void V1190B::ReadPairRes(PairRes &pairRes) {
        WriteMicro(Opcode(Command::READ_RES));
        uint16_t data = ReadMicro();
        pairRes.edgeTime = static_cast<ResLeadEdgeTime>(data & 0x0007);
        pairRes.width = static_cast<ResPulseWidth>((data & 0x0F00) >> 8U);
    }

    void V1190B::WriteDeadTime(DeadTime time) {
        WriteMicro(Opcode(Command::SET_DEAD_TIME));
        WriteMicro(static_cast<uint16_t>(time) & 0x0003);
    }

    V1190B::DeadTime V1190B::ReadDeadTime() {
        WriteMicro(Opcode(Command::READ_DEAD_TIME));
        return static_cast<DeadTime>(ReadMicro() & 0x0003);
    }

    // TEST AND DEBUG
    const std::array<uint8_t, 33> V1190B::fEEPROM = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                                     0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
                                                     0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
                                                     0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};

    void V1190B::WriteEEPROM(uint16_t address, uint16_t data) {
        if (std::find(fEEPROM.begin(), fEEPROM.end(), address) == fEEPROM.end()) {
            WriteMicro(Opcode(Command::WRITE_EEPROM));
            WriteMicro(address);
            WriteMicro(data & 0x00FF);
        } else
            PrintMessage(Message_t::ERROR, "Invalid EEPROM address");
    }

    uint16_t V1190B::ReadEEPROM(uint16_t address) {
        WriteMicro(Opcode(Command::READ_EEPROM));
        WriteMicro(address);
        return ReadMicro() & 0x00FF;
    }

    void V1190B::ReadMicroFirmware(MicroFirmware &firmware) {
        WriteMicro(Opcode(Command::REVISION_DATE_CONTROLLER_FW));
        firmware.revision = ReadMicro();
        firmware.day = ReadMicro();
        firmware.month = ReadMicro();
        firmware.year = ReadMicro();
    }

    void V1190B::WriteSpare(uint16_t data) {
        WriteMicro(Opcode(Command::WRITE_SPARE));
        WriteMicro(data);
    }

    uint16_t V1190B::ReadSpare() {
        WriteMicro(Opcode(Command::READ_SPARE));
        return ReadMicro();
    }

    void V1190B::EnableTestMode(uint32_t data) {
        if (ReadAcqMode() == TriggerMode_t::MATCHING) {
            WriteMicro(Opcode(Command::ENABLE_TEST_MODE));
            WriteMicro(data & 0x0000FFFF);
            WriteMicro((data >> 16U) & 0x000000FF);
        } else
            PrintMessage(Message_t::ERROR, "To use this function change trigger mode to TRIGGER MATCHING");
    }

    void V1190B::DisableTestMode() {
        WriteMicro(Opcode(Command::DISABLE_TEST_MODE));
    }

    void V1190B::WriteTDCTestOutput(uint16_t data) {
        WriteMicro(Opcode(Command::SET_TDC_TEST_OUTPUT));
        WriteMicro(data & 0x000F);
    }

    void V1190B::WriteDLLClock(DLLClock dllClock) {
        WriteMicro(Opcode(Command::SET_DLL_CLOCK));
        WriteMicro(static_cast<uint16_t>(dllClock) & 0x0003);
    }

    //CHANNEL
    void V1190B::WriteEnableChannel(uint8_t n, bool status) {
        if (status) WriteMicro(Opcode(Command::EN_CHANNEL, n % fChNumber));
        else WriteMicro(Opcode(Command::DIS_CHANNEL, n % fChNumber));
    }

    void V1190B::WriteEnableAll(bool status) {
        if (status) WriteMicro(Opcode(Command::EN_ALL_CH));
        else WriteMicro(Opcode(Command::DIS_ALL_CH));
    }


    void V1190B::WriteEnablePattern(V1190B::TDC tdc, uint32_t pattern) {
        switch (tdc) {
            case (TDC::TDC0) :
                WriteMicro(Opcode(Command::WRITE_EN_PATTERN32, 0x0000));
                break;
            case (TDC::TDC1) :
                WriteMicro(Opcode(Command::WRITE_EN_PATTERN32, 0x0001));
                break;
        }
        uint16_t lsb = (pattern & 0x0000ffff); // ch 0 : 15
        uint16_t msb = (pattern >> 16U);        // ch 16 : 31
        WriteMicro(lsb);
        WriteMicro(msb);
    }

    uint32_t V1190B::ReadEnablePattern(V1190B::TDC tdc) {
        switch (tdc) {
            case (TDC::TDC0) :
                WriteMicro(Opcode(Command::READ_EN_PATTERN32, 0x0000));
                break;
            case (TDC::TDC1) :
                WriteMicro(Opcode(Command::READ_EN_PATTERN32, 0x0001));
                break;
        }
        uint32_t lsb = ReadMicro(); // ch 0 : 15
        uint32_t msb = ReadMicro(); // ch 16 : 31

        return ((msb << 16U) & 0xffff0000) | (lsb & 0x0000ffff);
    }

    // TDC Readout
    void V1190B::WriteEnableHeaderTrailer(bool status) {
        if (status) WriteMicro(Opcode(Command::EN_HEAD_TRAILER));
        else WriteMicro(Opcode(Command::DIS_HEAD_TRAILER));
    }

    bool V1190B::ReadEnableHeaderTrailer() {
        WriteMicro(Opcode(Command::READ_HEAD_TRAILER));
        uint16_t data = ReadMicro();
        return (1U & data);
    }

    void V1190B::WriteMaxHitsPerEvent(V1190B::MaxHitsPerEvent n) {
        WriteMicro(Opcode(Command::SET_EVENT_SIZE));
        WriteMicro(static_cast<uint16_t>( n ));
    }

    V1190B::MaxHitsPerEvent V1190B::ReadMaxHitsPerEvent() {
        WriteMicro(Opcode(Command::READ_EVENT_SIZE));
        uint16_t data = ReadMicro();
        data &= 0x0F; // check 4 LSBs

        MaxHitsPerEvent result = MaxHitsPerEvent::INVALID;
        if (data <= static_cast<uint16_t>( MaxHitsPerEvent::NO_LIMIT )) {
            result = static_cast<MaxHitsPerEvent>( data );
        }
        return result;
    }

    void V1190B::WriteEnableErrMark(bool status) {
        if (status) WriteMicro(Opcode(Command::EN_ERROR_MARK));
        else WriteMicro(Opcode(Command::DIS_ERROR_MARK));
    }

    void V1190B::WriteEnableBypass(bool status) {
        if (status) WriteMicro(Opcode(Command::EN_ERROR_BYPASS));
        else WriteMicro(Opcode(Command::DIS_ERROR_BYPASS));
    }

    void V1190B::WriteErrorPattern(uint16_t pattern) {
        WriteMicro(Opcode(Command::SET_ERROR_TYPES));
        WriteMicro(pattern & IError_t::ALL);
    }

    uint16_t V1190B::ReadErrorPattern() {
        WriteMicro(Opcode(Command::READ_ERROR_TYPES));
        return ReadMicro() & IError_t::ALL; // return only 11 LSBs
    }

    void V1190B::WriteEffSizeFIFO(V1190B::FIFOSize size) {
        WriteMicro(Opcode(Command::SET_FIFO_SIZE));
        WriteMicro(static_cast<uint16_t>( size ));
    }

    uint16_t V1190B::ReadEffSizeFIFO() {
        WriteMicro(Opcode(Command::READ_FIFO_SIZE));
        uint16_t data = ReadMicro() & 0x0007; // save only 3 LSB
        return (2 << data); // starts with 2 (if data == 0) ---  not 1;
    }

    // MISC
    uint32_t V1190B::ReadTDC_ID(V1190B::TDC tdc) {
        switch (tdc) {
            case (TDC::TDC0) :
                WriteMicro(Opcode(Command::READ_TDC_ID, 0x00));
                break;
            case (TDC::TDC1) :
                WriteMicro(Opcode(Command::READ_TDC_ID, 0x01));
                break;
        }
        // NOTE : there is a typo in the doc
        // After this OPCODE is sent *TWO* 16-bit
        // words must be read
        uint32_t lsb = ReadMicro();
        uint32_t msb = ReadMicro();

        return ((msb << 16U) & 0xffff0000) | (lsb & 0x0000ffff);
    }

    uint16_t V1190B::ReadMicroFWRev() {
        WriteMicro(Opcode(Command::READ_MICRO_REV));
        return (ReadMicro() & 0x00FF);
    }

    void V1190B::WriteResetLoops() {
        WriteMicro(Opcode(Command::RESET_DLL_PLL));
    }

    //****** READING ROM + ******
    uint16_t V1190B::ReadVersion() {
        //Data seems to be a 1-byte long
        fVersion = ReadRegister16(V1190B_VERSION, 0x00FFU);
        return fVersion;
    }

    uint32_t V1190B::ReadBoardID() {
        //Data seems to be a 1-byte long
        uint32_t msb = ReadRegister16(V1190B_BOARD_ID_MSB, 0x00FF);
        uint32_t middle = ReadRegister16(V1190B_BOARD_ID, 0x00FF);
        uint32_t lsb = ReadRegister16(V1190B_BOARD_ID_LSB, 0x00FF);

        fBoardID = (lsb & 0xFFUL) | ((middle & 0xFFUL) << 8U) | ((msb & 0xFFUL) << 16);

        return fBoardID;
    }

    uint32_t V1190B::ReadOUI() {
        //Data seems to be a 1-byte long
        uint32_t msb = ReadRegister16(V1190B_OUI_MSB, 0x00FFU);
        uint32_t middle = ReadRegister16(V1190B_OUI, 0x00FFU);
        uint32_t lsb = ReadRegister16(V1190B_OUI_LSB, 0x00FFU);
        fOUI = (lsb) | (middle << 8U) | (msb << 16U);
        return fOUI;
    }

    uint32_t V1190B::ReadRevision() {
        //Data seems to be a 1-byte long
        uint32_t msb = ReadRegister16(V1190B_REVISION_MSB, 0x00FF);
        uint32_t mid1 = ReadRegister16(V1190B_REVISION_M1, 0x00FF);
        uint32_t mid2 = ReadRegister16(V1190B_REVISION_M2, 0x00FF);
        uint32_t lsb = ReadRegister16(V1190B_REVISION_LSB, 0x00FF);
        fRevision = (lsb) | (mid2 << 8U) | (mid1 << 12U) | (msb << 16U);
        return fRevision;
    }

    uint16_t V1190B::ReadSerialNumber() {
        uint16_t msb = ReadRegister16(V1190B_SERIAL_MSB, 0x00FFU);
        uint16_t lsb = ReadRegister16(V1190B_SERIAL_LSB, 0x00FFU);
        fSerial = (lsb & 0x00FFU) | (msb << 8U);
        return fSerial;
    }

    uint16_t V1190B::ReadFirmRevNumber() {
        uint16_t data = ReadRegister16(V1190B_FIRMWARE_REVISION);
        fFirmwareRevision = std::to_string((data & 0x00F0U) >> 4U) + "." +
                            std::to_string((data & 0x000FU));
        fFirmware = fFirmwareRevision;
        return data;
    }

    //****** READING ROM - ******
    void V1190B::WriteDummy32(uint32_t data) {
        WriteRegister32(V1190B_DUMMY_32, data);
    }

    uint32_t V1190B::ReadDummy32() {
        return ReadRegister32(V1190B_DUMMY_32);
    }

    void V1190B::WriteDummy16(uint16_t data) {
        WriteRegister16(V1190B_DUMMY_16, data);
    }

    uint16_t V1190B::ReadDummy16() {
        return ReadRegister16(V1190B_DUMMY_16);
    }

    void V1190B::WritePOut(V1190B::ProgOUT pout) {
        WriteRegister16(V1190B_OUT_PROG_CONTROL, static_cast<uint16_t>( pout ), 0x0003);
    }

    V1190B::ProgOUT V1190B::ReadPOut() {
        uint16_t data = ReadRegister16(V1190B_OUT_PROG_CONTROL, 0x0003);
        return static_cast<ProgOUT>( data );
    }

    //LUT
    void V1190B::EnableReadoutSRAM(bool status) {
        if (status)
            SetBit16(V1190B_CONTROL_REGISTER, V1190B_EN_LUT_READ_BIT);
        else
            ClearBit16(V1190B_CONTROL_REGISTER, V1190B_EN_LUT_READ_BIT);
    }

    void V1190B::ReadCompensation(TrLeadLSB lsb, uint8_t channel, std::vector <int8_t> &data) {
        channel %= fChNumber;
        data.clear();
        uint32_t bytes = 0;
        bool status = ReadRegister16(V1190B_CONTROL_REGISTER, V1190B_EN_LUT_READ_BIT);
        EnableReadoutSRAM(true);
        switch (lsb) {
            case TrLeadLSB::ps100:
                bytes = 256;
                break;
            case TrLeadLSB::ps200:
                channel += 128;
                bytes = 128;
                break;
            default:
                PrintMessage(Message_t::WARNING,
                             "There is no LUT for the provided resolution. Choose either 100ps or 200ps resolution.");
                break;
        }
        WriteRegister16(V1190B_COMPENSATION_SRAM_PAGE, channel);
        for (uint32_t i = 0; i < bytes; ++i)
            data.push_back(ReadRegister16(V1190B_COMPENSATION_SRAM + i) & 0x00FF);
        EnableReadoutSRAM(status);

    }
}
