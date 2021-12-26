#include "engine/maths/vec2.h"
#include "sb/collider.h"

namespace SB
{
    class PhysicsBody
    {
    public:
        Engine::Vec2 pos;
        Engine::Vec2 vel;
        Engine::Vec2 acc;
        float mass;
        float drag;
        Collider* const collider;

    private:
        Engine::Vec2 m_force_sum;

    public:
        PhysicsBody(const Engine::Vec2& pos, const float mass, const float drag, Collider* const collider);

        void add_force(const Engine::Vec2& force);
        void update(const float elapsed);
    };
}
