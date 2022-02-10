#ifndef V_PLUS_MOODLE
#define V_PLUS_MOODLE

#include <cstddef>
#include <cstdint>

namespace vmepp
{
    typedef uint32_t DataWord_t;

    constexpr size_t gMaxBLT    = 2048;
    constexpr size_t gMaxNBLT   = 32;

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
