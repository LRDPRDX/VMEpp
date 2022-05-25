#ifndef V_PLUS_V1742B_SRLZ_H
#define V_PLUS_V1742B_SRLZ_H

namespace vmepp
{
    //****************************
    //****** SAMPLINGRATE_T ******
    //****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::SamplingRate_t const& value )
    {
        switch( value )
        {
            case( V1742B::SamplingRate_t::M5000 ) : return "5GHz"; break;
            case( V1742B::SamplingRate_t::M2500 ) : return "2_5GHz"; break;
            case( V1742B::SamplingRate_t::M1000 ) : return "1GHz"; break;
            case( V1742B::SamplingRate_t::M750 )  : return "750MHz"; break;
        }
        return "5GHz";//Default value
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::SamplingRate_t& value, std::string const& str )
    {
        if     ( str == "5GHz" )    { value = V1742B::SamplingRate_t::M5000; }
        else if( str == "2_5GHz" )  { value = V1742B::SamplingRate_t::M2500; }
        else if( str == "1GHz" )    { value = V1742B::SamplingRate_t::M1000; }
        else if( str == "750MHz" )  { value = V1742B::SamplingRate_t::M750;  }
        else                        { value = V1742B::SamplingRate_t::M5000; }//Default value
    }

    //****************************
    //****** RECORDLENGTH_T ******
    //****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::RecordLength_t const& value )
    {
        switch( value )
        {
            case( V1742B::RecordLength_t::s1024 ) : return "1024"; break;
            case( V1742B::RecordLength_t::s520 )  : return "520"; break;
            case( V1742B::RecordLength_t::s256 )  : return "256"; break;
            case( V1742B::RecordLength_t::s136 )  : return "136"; break;
        }
        return "1024";//Default value
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::RecordLength_t& value, std::string const& str )
    {
        if     ( str == "1024" )    { value = V1742B::RecordLength_t::s1024; }
        else if( str == "520" )     { value = V1742B::RecordLength_t::s520; }
        else if( str == "256" )     { value = V1742B::RecordLength_t::s256; }
        else if( str == "136" )     { value = V1742B::RecordLength_t::s136;  }
        else                        { value = V1742B::RecordLength_t::s1024; }//Default value
    }

    //*****************************
    //****** GLOBALTRIGGER_T ******
    //*****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::GlobalTrigger_t const& value )
    {
        switch( value )
        {
            case( V1742B::GlobalTrigger_t::None )           : return "none"; break;
            case( V1742B::GlobalTrigger_t::ExternalOnly )   : return "external_only"; break;
            case( V1742B::GlobalTrigger_t::SWOnly )         : return "sw_only"; break;
            case( V1742B::GlobalTrigger_t::All )            : return "all"; break;
        }
        return "none";//Default value
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::GlobalTrigger_t& value, std::string const& str )
    {
        if     ( str == "none" )            { value = V1742B::GlobalTrigger_t::None; }
        else if( str == "external_only" )   { value = V1742B::GlobalTrigger_t::ExternalOnly; }
        else if( str == "sw_only" )         { value = V1742B::GlobalTrigger_t::SWOnly; }
        else if( str == "all" )             { value = V1742B::GlobalTrigger_t::All;  }
        else                                { value = V1742B::GlobalTrigger_t::None; }//Default value
    }

    //*******************************
    //****** TRIGGERPOLARITY_T ******
    //*******************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::TriggerPolarity_t const& value )
    {
        switch( value )
        {
            case( V1742B::TriggerPolarity_t::RisingEdge )   : return "rising_edge"; break;
            case( V1742B::TriggerPolarity_t::FallingEdge )  : return "falling_edge"; break;
        }
        return "falling_edge";
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TriggerPolarity_t& value, std::string const& str )
    {
        if     ( str == "rising_edge" )     { value = V1742B::TriggerPolarity_t::RisingEdge; }
        else if( str == "falling_edge" )    { value = V1742B::TriggerPolarity_t::FallingEdge; }
        else                                { value = V1742B::TriggerPolarity_t::FallingEdge; }
    }

    //************************
    //****** LEMO_LEVEL ******
    //************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::Level_t const& value )
    {
        switch( value )
        {
            case( V1742B::Level_t::NIM )    : return "NIM"; break;
            case( V1742B::Level_t::TTL )    : return "TTL"; break;
        }
        return "TTL";
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::Level_t& value, std::string const& str )
    {
        if     ( str == "NIM" ) { value = V1742B::Level_t::NIM; }
        else if( str == "TTL" ) { value = V1742B::Level_t::TTL; }
        else                    { value = V1742B::Level_t::TTL; }
    }

    //**********************
    //****** ACQ_MODE ******
    //**********************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::AcqMode_t const& value )
    {
        switch( value )
        {
            case( V1742B::AcqMode_t::Transparent )  : return "transparent"; break;
            case( V1742B::AcqMode_t::Output )       : return "output"; break;
        }
        return "transparent";
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::AcqMode_t& value, std::string const& str )
    {
        if     ( str == "transparent" ) { value = V1742B::AcqMode_t::Transparent; }
        else if( str == "output" )      { value = V1742B::AcqMode_t::Output; }
        else                            { value = V1742B::AcqMode_t::Transparent; }
    }

    //***************************
    //****** TRG_IN_SIGNAL ******
    //***************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::TrgInSignal_t const& value )
    {
        switch( value )
        {
            case( V1742B::TrgInSignal_t::Gate )  : return "gate"; break;
            case( V1742B::TrgInSignal_t::Veto )  : return "veto"; break;
        }
        return "gate";
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TrgInSignal_t& value, std::string const& str )
    {
        if     ( str == "gate" )    { value = V1742B::TrgInSignal_t::Gate; }
        else if( str == "veto" )    { value = V1742B::TrgInSignal_t::Veto; }
        else                        { value = V1742B::TrgInSignal_t::Gate; }
    }

    //******************************
    //****** TRG_IN_SYNC_TYPE ******
    //******************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::TrgInSync_t const& value )
    {
        switch( value )
        {
            case( V1742B::TrgInSync_t::Edge )       : return "edge"; break;
            case( V1742B::TrgInSync_t::Duration )   : return "duration"; break;
        }
        return "edge";
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TrgInSync_t& value, std::string const& str )
    {
        if     ( str == "edge" )        { value = V1742B::TrgInSync_t::Edge; }
        else if( str == "duration" )    { value = V1742B::TrgInSync_t::Duration; }
        else                            { value = V1742B::TrgInSync_t::Edge; }
    }

    //****************************
    //****** TRG-OUT_SIGNAL ******
    //****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::TrgOutSignal_t const& value )
    {
        switch( value )
        {
            case( V1742B::TrgOutSignal_t::NoSignal )    : return "none"; break;
            case( V1742B::TrgOutSignal_t::AllTRn )      : return "all_TRn"; break;
            case( V1742B::TrgOutSignal_t::AcceptedTRn ) : return "accepted_TRn"; break;
            case( V1742B::TrgOutSignal_t::BusyGroups )  : return "busy_groups"; break;
        }
        return "none";//Default value
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TrgOutSignal_t& value, std::string const& str )
    {
        if     ( str == "none" )            { value = V1742B::TrgOutSignal_t::NoSignal; }
        else if( str == "all_TRn" )         { value = V1742B::TrgOutSignal_t::AllTRn; }
        else if( str == "accepted_TRn" )    { value = V1742B::TrgOutSignal_t::AcceptedTRn; }
        else if( str == "busy_groups" )     { value = V1742B::TrgOutSignal_t::BusyGroups;  }
        else                                { value = V1742B::TrgOutSignal_t::BusyGroups; }//Default value
    }

    //***************************
    //****** STARTSOURCE_T ******
    //***************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::StartSource_t const& value )
    {
        switch( value )
        {
            case( V1742B::StartSource_t::SW )       : return "sw"; break;
            case( V1742B::StartSource_t::S_IN )     : return "s_in"; break;
            case( V1742B::StartSource_t::FirstTrg ) : return "first_trg"; break;
            case( V1742B::StartSource_t::LVDS )     : return "LVDS"; break;
        }
        return "sw";//Default value
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::StartSource_t& value, std::string const& str )
    {
        if     ( str == "sw" )          { value = V1742B::StartSource_t::SW; }
        else if( str == "s_in" )        { value = V1742B::StartSource_t::S_IN; }
        else if( str == "first_trg" )   { value = V1742B::StartSource_t::FirstTrg; }
        else if( str == "LVDS" )        { value = V1742B::StartSource_t::LVDS;  }
        else                            { value = V1742B::StartSource_t::SW; }//Default value
    }
}
#endif
