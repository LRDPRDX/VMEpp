#ifndef V_PLUS_EXCEPTION_H
#define V_PLUS_EXCEPTION_H

#include <type_traits>
#include <iostream>
#include <exception>
#include <string>
#include <array>

#include "CAENVMEtypes.h"


namespace vmepp
{
    enum class VError_t {   vSuccess = cvSuccess,
                            vBusError = cvBusError,
                            vCommError = cvCommError,
                            vGenericError = cvGenericError,
                            vInvalidParam = cvInvalidParam,
                            vTimeoutError = cvTimeoutError,
                            vAlreadyOpenError = cvAlreadyOpenError,
                            vMaxBoardCountError = cvMaxBoardCountError,
                            vBadMaster = -10,
                            vOrphan = -11,
                            vBuffAllocFailed = -12,
                            vBadSlave = -13,
                            vConfigError = -14,
                            vAccessError = -15
                        };

    enum class Message_t {  INFO,
                            WARNING,
                            ERROR };

    class VException : public std::exception
    {
        protected :
            VError_t            fErrorCode;

            std::string         fMessage;
            std::string         fInfo;
            std::string         fHint;

        public :
            VException( VError_t errorCode, const std::string &addInfo ):
                fErrorCode( errorCode ),
                fMessage( "" ),
                fInfo( addInfo ),
                fHint( "N/A" )
            {
                switch( errorCode )
                {
                    case( VError_t::vSuccess ) :
                        fMessage = "Operation completed successfully"; break;
                    case( VError_t::vBusError ) :
                        fMessage = "VME bus error during the cycle";
                        fHint    = "Check your cable or reboot VME crate manually";
                        break;
                    case( VError_t::vCommError ) :
                        fMessage = "Communication error"; break;
                    case( VError_t::vGenericError ) :
                        fMessage = "Unspecified error"; break;
                    case( VError_t::vInvalidParam ) :
                        fMessage = "Invalid parameter"; break;
                    case( VError_t::vTimeoutError ) :
                        fMessage = "Timeout error"; break;
                    case( VError_t::vAlreadyOpenError ) :
                        fMessage = "Device is already open error"; break;
                    case( VError_t::vMaxBoardCountError ) :
                        fMessage = "Maximum device number has been reached"; break;
                    case( VError_t::vBadMaster ) :
                        fMessage = "Requesting a bad controller";
                        fHint    = "Try to register this board to the correct master";
                        break;
                    case( VError_t::vOrphan ) :
                        fMessage = "The controller is invalid";
                        fHint = "Use VController::RegisterSlave( VSlave* ) method to register the board";
                        break;
                    case( VError_t::vBuffAllocFailed ) :
                        fMessage = "Failed to allocate memory for the readout buffer";
                        break;
                    case( VError_t::vBadSlave ) :
                        fMessage = "Bad master (base address, nullptr, etc.)";
                        break;
                    case( VError_t::vConfigError ) :
                        fMessage = "Error occured while writing/reading config";
                        fHint = "To see how a particular config should look like try to write the default config: WriteConfigToFile( UConfig<T>(), \"path\" ), where \'T\' is your module class";
                        break;
                    case( VError_t::vAccessError ) :
                        fMessage = "Data access error";
                        fHint = "Check the permissions";
                        break;
                }
            }

            enum class Prefix_t { MESSAGE, INFO, HINT };

            static std::string Wrap( Prefix_t p, const std::string& msg )
            {
                std::string res = msg;

                switch( p )
                {
                    case( Prefix_t::MESSAGE ) :
                        res = "VME++ :: ERROR :: " + res; break;
                    case( Prefix_t::INFO ) :
                        res = "VME++ :: INFO :: " + res; break;
                    case( Prefix_t::HINT ) :
                        res = "VME++ :: HINT :: " + res; break;
                }

                return res;
            }

            virtual ~VException() throw() { };

            virtual const char* what() const throw()
            {
                return fMessage.c_str();
            }

            std::string GetErrorMessage() const throw()
            {
                return Wrap( Prefix_t::MESSAGE, fMessage );
            }

            std::string GetInfo() const throw()
            {
                return Wrap( Prefix_t::INFO, fInfo );
            }

            std::string GetHint() const throw()
            {
                return Wrap( Prefix_t::HINT, fHint );
            }

            VError_t GetErrorCode() const { return fErrorCode; }
    };

    inline void PrintMessage( Message_t mType, const std::string &msg )
    {
        std::string prefix = "VME++";
        switch( mType )
        {
            case( Message_t::INFO ) :
                prefix = "\033[1;32mINFO :: " + prefix ;//Green colored text
                break;
            case( Message_t::WARNING ) :
                prefix = "\033[1;33mWARNING :: " + prefix;//Yellow colored text
                break;
            case( Message_t::ERROR ) :
                prefix = "\033[1;31mERROR :: " + prefix;//Red colored text
                break;
        }
        std::cerr << prefix << " : " << msg << "\033[0;0m" << std::endl;
    }

    template <typename T, size_t N>
    inline void WriteStdArrayToStream( const std::array<T,N>& ar, std::ostream& stream )
    {
        stream.write( reinterpret_cast<char const*>( ar.data() ),
                      ar.size() * sizeof( T ) );
    }

    template <typename T, size_t N>
    inline void ReadStdArrayFromStream( std::array<T,N>& ar, std::istream& stream )
    {
        stream.read( reinterpret_cast<char*>( ar.data() ),
                     ar.size() * sizeof( T ) );
    }

    template <typename ENUM_TYPE, ENUM_TYPE First, ENUM_TYPE Last>
    class EnumIterator
    {
        typedef typename std::underlying_type<ENUM_TYPE>::type val_t;
        int val;

        public:
            EnumIterator( const ENUM_TYPE& v ) :
                val( static_cast<val_t>( v ) )
            { }
            EnumIterator() :
                val( static_cast<val_t>( First ) )
            { }

        EnumIterator operator++()
        {
            ++val;
            return *this;
        }

        ENUM_TYPE operator*() { return static_cast<ENUM_TYPE>( val ); }

        EnumIterator begin() { return *this; }
        EnumIterator end()
        {
            static const EnumIterator endIter = ++EnumIterator( Last );
            return endIter;
        }

        bool operator!=( const EnumIterator& i ) { return val != i.val; }
    };
}
#endif//V_PLUS_EXCEPTION_H
