#include <stdint.h>

namespace SB
{
    class Game
    {
    private:
        uint32_t m_target_ticks;
        uint32_t m_prev_ticks;
        uint32_t m_tick_diff;
        float m_elapsed;

    public:
        Game(const unsigned int target_fps);
        void run();

    private:
        void limit_fps();
    };
}
