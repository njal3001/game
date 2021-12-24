#include "engine/maths/vec2.h"
#include "sb/collider.h"

namespace SB
{
    class PhysicsBody
    {
    private:
        Engine::Vec2 m_pos;
        Engine::Vec2 m_vel;
        Engine::Vec2 m_acc;
        float m_mass;
        float m_drag;
        Collider* m_collider;

    public:
        PhysicsBody(const Engine::Vec2& pos, const float mass, const float drag, Collider* collider);

        /* PhysicsBody(const PhysicsBody& other) = delete; */
        /* PhysicsBody& operator=(const PhysicsBody& other) = delete; */

        void move(const Engine::Vec2& amount);
        void add_force(const Engine::Vec2& force);
        void update(const float elapsed);
    };
}
