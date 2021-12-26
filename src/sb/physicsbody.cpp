#include "sb/physicsbody.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    PhysicsBody::PhysicsBody(const Vec2& pos, const float mass, const float drag, Collider* const collider)
        : pos(pos), vel(Vec2::zero), acc(Vec2::zero), mass(mass), drag(drag), collider(collider), m_force_sum(Vec2())
    {}


    void PhysicsBody::add_force(const Vec2& force)
    {
        m_force_sum += force;
    }

    void PhysicsBody::update(const float elapsed)
    {
        Vec2 total_acc = (m_force_sum / mass) + acc;

        pos += vel * elapsed;
        vel += total_acc * elapsed;
        vel *= drag; // TODO: Not frame rate independent

        if (vel.len_squared() < 0.01f)
        {
            vel = Vec2::zero;
        }

        m_force_sum = Vec2::zero;
    }
}
