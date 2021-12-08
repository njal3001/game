#include <stdint.h>

namespace Game
{
    class Game
    {
    private:
        uint32_t m_target_ticks;
        uint32_t m_prev_ticks;
        uint32_t m_tick_diff;

    public:
        Game(const unsigned int target_fps);
        void run();

    private:
        void limit_fps();
    };
}
