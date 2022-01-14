#include "sb/bullet.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& vel)
        : vel(vel)
    {}

    void Bullet::render(Engine::Renderer* renderer)
    {
        const CircleCollider* collider = (CircleCollider*)get<Collider>();
        renderer->circ(m_entity->pos, collider->bounds.radius, 128, Color(0, 255, 255));
    }

    Entity* Bullet::create(Scene* scene, const Engine::Vec2& pos, 
            const Engine::Vec2& vel, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Bullet(vel));

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        e->add(c);

        Mover* m = new Mover();
        m->collider = c;
        m->vel = vel;
        m->stop_mask |= (Mask::Player | Mask::PlayerDash);

        m->on_hit = [](Mover* mover, Collider* other, const Vec2& disp)
        {
            if (other->mask & (Mask::Solid | Mask::PlayerDash))
            {
                mover->entity()->destroy();
                if (other->mask & Mask::PlayerDash)
                {
                    printf("Bullet dash collision!\n");
                }
            }
            else if (other->mask & Mask::Player)
            {
                mover->entity()->destroy();
                other->entity()->get<Player>()->hurt();
            }
        };

        e->add(m);

        return e;
    }
}
