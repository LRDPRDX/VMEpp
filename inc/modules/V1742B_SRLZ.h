#ifndef V_PLUS_V1742B_SRLZ_H
#define V_PLUS_V1742B_SRLZ_H

#include <sstream>
#include <string>
#include <iomanip>


namespace vmepp
{
    //****************************
    //****** SAMPLINGRATE_T ******
    //****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::SamplingRate_t const& value )
    {
        std::string s;
        switch( value )
        {
            case( V1742B::SamplingRate_t::M5000 ) : s = "5GHz"; break;
            case( V1742B::SamplingRate_t::M2500 ) : s = "2_5GHz"; break;
            case( V1742B::SamplingRate_t::M1000 ) : s = "1GHz"; break;
            case( V1742B::SamplingRate_t::M750 )  : s = "750MHz"; break;
            default                               : s = "5GHz";                                       
        }
        return s;
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
        std::string s;
        switch( value )
        {
            case( V1742B::RecordLength_t::s1024 ) : s = "1024"; break;
            case( V1742B::RecordLength_t::s520 )  : s = "520"; break;
            case( V1742B::RecordLength_t::s256 )  : s = "256"; break;
            case( V1742B::RecordLength_t::s136 )  : s = "136"; break;
            default                               : s = "1024";
        }
        return s;
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
        std::string s;
        switch( value )
        {
            case( V1742B::GlobalTrigger_t::None )           : s = "none"; break;
            case( V1742B::GlobalTrigger_t::ExternalOnly )   : s = "external_only"; break;
            case( V1742B::GlobalTrigger_t::SWOnly )         : s = "sw_only"; break;
            case( V1742B::GlobalTrigger_t::All )            : s = "all"; break;
            default                                         : s = "none";
        }
        return s; 
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
        std::string s;
        switch( value )
        {
            case( V1742B::TriggerPolarity_t::RisingEdge )   : s = "rising_edge"; break;
            case( V1742B::TriggerPolarity_t::FallingEdge )  : s = "falling_edge"; break;
            default                                         : s = "rising_edge";
        }
        return s; 
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TriggerPolarity_t& value, std::string const& str )
    {
        if     ( str == "rising_edge" )     { value = V1742B::TriggerPolarity_t::RisingEdge; }
        else if( str == "falling_edge" )    { value = V1742B::TriggerPolarity_t::FallingEdge; }
        else                                { value = V1742B::TriggerPolarity_t::RisingEdge; }
    }

    //************************
    //****** LEMO_LEVEL ******
    //************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::Level_t const& value )
    {
        std::string s;
        switch( value )
        {
            case( V1742B::Level_t::NIM )    : s = "NIM"; break;
            case( V1742B::Level_t::TTL )    : s = "TTL"; break;
            default                         : s = "NIM";
        }
        return s;
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::Level_t& value, std::string const& str )
    {
        if     ( str == "NIM" ) { value = V1742B::Level_t::NIM; }
        else if( str == "TTL" ) { value = V1742B::Level_t::TTL; }
        else                    { value = V1742B::Level_t::NIM; }
    }

    //**********************
    //****** ACQ_MODE ******
    //**********************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::AcqMode_t const& value )
    {
        std::string s;
        switch( value )
        {
            case( V1742B::AcqMode_t::Transparent )  : s = "transparent"; break;
            case( V1742B::AcqMode_t::Output )       : s = "output"; break;
            default                                 : s = "transparent";
        }
        return s;
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
        std::string s;
        switch( value )
        {
            case( V1742B::TrgInSignal_t::Gate )  : s = "gate"; break;
            case( V1742B::TrgInSignal_t::Veto )  : s = "veto"; break;
            default                              : s = "gate";                                                   
        }
        return s;
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
        std::string s;
        switch( value )
        {
            case( V1742B::TrgInSync_t::Edge )       : s = "edge"; break;
            case( V1742B::TrgInSync_t::Duration )   : s = "duration"; break;
            default                                 : s = "edge";
        }
        return s;
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
        std::string s;
        switch( value )
        {
            case( V1742B::TrgOutSignal_t::NoSignal )    : s = "none"; break;
            case( V1742B::TrgOutSignal_t::AllTRn )      : s = "all_TRn"; break;
            case( V1742B::TrgOutSignal_t::AcceptedTRn ) : s = "accepted_TRn"; break;
            case( V1742B::TrgOutSignal_t::BusyGroups )  : s = "busy_groups"; break;
            default                                     : s = "none";
        }
        return s;
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TrgOutSignal_t& value, std::string const& str )
    {
        if     ( str == "none" )            { value = V1742B::TrgOutSignal_t::NoSignal; }
        else if( str == "all_TRn" )         { value = V1742B::TrgOutSignal_t::AllTRn; }
        else if( str == "accepted_TRn" )    { value = V1742B::TrgOutSignal_t::AcceptedTRn; }
        else if( str == "busy_groups" )     { value = V1742B::TrgOutSignal_t::BusyGroups;  }
        else                                { value = V1742B::TrgOutSignal_t::NoSignal; }//Default value
    }

    //***************************
    //****** STARTSOURCE_T ******
    //***************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::StartSource_t const& value )
    {
        std::string s;
        switch( value )
        {
            case( V1742B::StartSource_t::SW )       : s = "sw"; break;
            case( V1742B::StartSource_t::S_IN )     : s = "s_in"; break;
            case( V1742B::StartSource_t::FirstTrg ) : s = "first_trg"; break;
            case( V1742B::StartSource_t::LVDS )     : s = "LVDS"; break;
            default                                 : s = "sw";
        }
        return s;
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

    //****************************
    //****** TRG-OUT-SOURCE ******
    //****************************
    template <class TArchive>
    std::string save_minimal( TArchive const&, V1742B::TrgOutSource_t const& value )
    {
        std::stringstream ss;
        ss << "0x"
           << std::setfill( '0' )
           << std::setw( 2 * sizeof( std::underlying_type<V1742B::TrgOutSource_t>::type ) )
           << std::hex
           << static_cast<std::underlying_type<V1742B::TrgOutSource_t>::type>( value );
        return ss.str();  
    }

    template <class TArchive>
    void load_minimal( TArchive const&, V1742B::TrgOutSource_t& value, std::string const& str )
    {
        try
        {
            value = static_cast<V1742B::TrgOutSource_t>( std::stol( str, nullptr, 16 ) );
        }
        catch( const std::exception& e )
        {
            value = V1742B::TrgOutSource_t::None;
        }
    }
}
#endif
