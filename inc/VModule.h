#ifndef V_PLUS_MOODLE
#define V_PLUS_MOODLE

namespace vmepp
{
    constexpr unsigned MaxBlockTransferSize = 2048;

    class VModule
    {
        public :
            VModule() {}
            virtual ~VModule() = default;
            VModule( const VModule &other ) = delete;
            VModule& operator=( const VModule &other ) = delete;
    };
}
#endif
