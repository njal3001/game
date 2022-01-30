#include "sb/bullet.h"
#include "sb/boxcollider.h"
#include "sb/mover.h"

namespace SB
{
    using namespace Engine;

    Entity* Bullet::create(Scene* scene, const Vec2& pos, 
            const Vec2& vel, const float radius)
    {
        Entity* e = scene->add_entity(pos);
        e->add(new Bullet());

        Collider* c = new CircleCollider(Circ(Vec2(), radius));
        c->visible = true;
        c->trigger_only = true;
        c->color = Color(0, 255, 255);
        e->add(c);

        Mover* m = new Mover();
        m->collider = c;
        m->vel = vel;
        m->stop_mask |= (Mask::PlayerDash | Mask::Player);

        m->on_hit = [](Mover* mover, Collider* other, const Vec2& dir)
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
