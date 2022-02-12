#include <stdint.h>
#include "ecs.h"

namespace SB
{
    class Game
    {
    private:
        uint64_t m_target_ticks;
        uint64_t m_prev_ticks;
        float m_elapsed; // In seconds

        static constexpr int tile_size = 8;

    public:
        Game(const unsigned int target_fps);
        void run();

    private:
        void limit_fps();

        static Entity* create_wall(Scene* scene, const int x, const int y,
            const int w, const int h);
    };
}
