#ifndef V_PLUS_MOODLE
#define V_PLUS_MOODLE

namespace vmepp
{
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
