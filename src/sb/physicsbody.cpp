#include "sb/physicsbody.h"
#include <assert.h>

namespace SB
{
    using namespace Engine;

    PhysicsBody::PhysicsBody(const Vec2& pos, const float mass, const float drag, Collider* collider)
        : m_pos(pos), m_vel(Vec2()), m_acc(Vec2()), m_mass(mass), m_drag(drag), m_collider(collider)
    {
        assert(mass > 0.0f);
        assert(drag > 0.0f && drag <= 1.0f);
        assert(collider);
    }

    void PhysicsBody::move(const Vec2& amount)
    {
        m_pos += amount;
    }

    void PhysicsBody::add_force(const Vec2& force)
    {
        m_acc += force / m_mass;
    }

    void PhysicsBody::update(const float elapsed)
    {
        m_pos += m_vel * elapsed;

        m_vel += m_acc * elapsed;
        m_acc *= m_drag; // TODO: Not frame rate independent

        if (m_acc.len_squared() < 0.01f)
        {
            m_acc.x = 0.0f;
            m_acc.y = 0.0f;
        }

        if (m_vel.len_squared() < 0.01f)
        {
            m_vel.x = 0.0f;
            m_vel.y = 0.0f;
        }
    }
}
