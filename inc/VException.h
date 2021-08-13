#ifndef V_PLUS_EXCEPTION_H
#define V_PLUS_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

#include "CAENVMEtypes.h"


namespace vmeplus
{
    enum class VError_t {   vSuccess = cvSuccess,
                            vBusError = cvBusError,
                            vCommError = cvCommError,
                            vGenericError = cvGenericError,
                            vInvalidParam = cvInvalidParam,
                            vTimeoutError = cvTimeoutError,
                            vBadMaster = -10,
                            vOrphan = -11,
                            vBuffAllocFailed = -12,
                            vBadSlave = -13,
                            vConfigError = -14,
                            vAccessError = 15
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
                        fMessage = "VME++ :: ERROR :: Operation completed successfully"; break;
                    case( VError_t::vBusError ) :
                        fMessage = "VME++ :: ERROR :: VME bus error during the cycle";
                        fHint    = "VME++ :: HINT :: Check your cable or reboot VME crate manually";
                        break;
                    case( VError_t::vCommError ) :
                        fMessage = "VME++ :: ERROR :: Communication error"; break;
                    case( VError_t::vGenericError ) :      
                        fMessage = "VME++ :: ERROR :: Unspecified error"; break;
                    case( VError_t::vInvalidParam ) :      
                        fMessage = "VME++ :: ERROR :: Invalid parameter"; break;
                    case( VError_t::vTimeoutError ) :      
                        fMessage = "VME++ :: ERROR :: Timeout error"; break;
                    case( VError_t::vBadMaster ) :
                        fMessage = "VME++ :: ERROR :: Requesting a bad controller";
                        fHint    = "VME++ :: HINT :: Try to register this board to the correct master";
                        break;
                    case( VError_t::vOrphan ) :
                        fMessage = "VME++ :: ERROR :: The controller is invalid";
                        fHint = "VME++ :: HINT :: Use VController::RegisterBoard( VBoard* ) method to register the board";
                        break;
                    case( VError_t::vBuffAllocFailed ) :
                        fMessage = "VME++ :: ERROR :: Failed to allocate memory for the readout buffer";
                        break;
                    case( VError_t::vBadSlave ) :
                        fMessage = "VME++ :: ERROR :: Bad master (base address, nullptr, etc.)";
                        break;
                    case( VError_t::vConfigError ) :
                        fMessage = "VME++ :: ERROR :: Error occured while writing/reading JSON config";
                        fHint = "VME++ :: HINT :: Call ::GetDefaultConfig() member function to see how a config should look";
                    case( VError_t::vAccessError ) :
                        fMessage = "VME++ :: ERROR :: Data access error";
                        fHint = "VME++ :: HINT :: Check the permissions";
                }
            }

            virtual ~VException() throw() { };

            virtual const char* what() const throw()
            {
                return fMessage.c_str();
            }

            std::string GetErrorMessage() const throw()
            {
                return fMessage;
            }

            std::string GetInfo() const throw()
            {
                return fInfo;
            }

            std::string GetHint() const throw()
            {
                return fHint;
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
}
#endif//V_PLUS_EXCEPTION_H
