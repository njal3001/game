#include "sb/bullet.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"
#include "sb/charger.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const float lifetime)
        : m_lifetime(lifetime)
    {}

    void Bullet::update(const float elapsed)
    {
        m_lifetime -= elapsed;

        if (m_lifetime <= 0.0f)
        {
            m_entity->destroy();
        }
    }

    Entity* Bullet::create(Scene* scene, const Vec2& pos, 
            const Vec2& vel, const float lifetime, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Bullet(lifetime));

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        c->visible = true;
        c->trigger_only = true;
        c->color = Color(0, 255, 255);
        e->add(c);

        Mover* m = new Mover();
        m->collider = c;
        m->vel = vel;
        m->stop_mask |= (Mask::Enemy);

        m->on_hit = [](Mover* mover, Collider* other, const Vec2& dir)
        {
            if (other->mask & Mask::Enemy)
            {
                other->get<Charger>()->hurt(-dir);
            }

            mover->entity()->destroy();
        };

        e->add(m);

        return e;
    }
}
