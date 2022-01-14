#include "sb/ecs.h"
#include <functional>

namespace SB
{
    class Mover : public Component
    {
    public:
        Collider* collider;
        Engine::Vec2 vel;
        uint32_t stop_mask;
        std::function<void (Mover* mover, Collider* other, const Engine::Vec2& disp)> on_hit;

        Mover();

        void update(const float elapsed) override;
    };
}
