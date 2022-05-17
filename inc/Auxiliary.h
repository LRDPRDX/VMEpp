#ifndef V_PLUS_AUX 
#define V_PLUS_AUX

#include <type_traits>
#include <array>
#include <string>


namespace vmepp
{
    enum class Message_t {  INFO,
                            WARNING,
                            ERROR };

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
    inline void WriteStdArrayToBinaryStream( const std::array<T,N>& ar, std::ostream& stream )
    {
        static_assert( std::is_pod<T>::value, "T MUST be POD" );
        stream.write( reinterpret_cast<char const*>( ar.data() ),
                      ar.size() * sizeof( T ) );
    }

    template <typename T, size_t N>
    inline void ReadStdArrayFromBinaryStream( std::array<T,N>& ar, std::istream& stream )
    {
        static_assert( std::is_pod<T>::value, "T MUST be POD" );
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
#endif //V_PLUS_AUX
