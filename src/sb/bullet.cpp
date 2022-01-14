#include "sb/bullet.h"
#include "sb/boxcollider.h"

namespace SB
{
    using namespace Engine;

    Bullet::Bullet(const Vec2& vel)
        : vel(vel)
    {}

    void Bullet::update(const float elapsed)
    {
        const Collider* collider = get<Collider>();

        Player* player = scene()->first<Player>();
        const Collider* p_collider = player->get<Collider>();

        if (player && collider->intersects(*p_collider))
        {
            // Collided with player
            if (!player->dashing())
            {
                player->hurt();
                destroy();

                printf("Collided with player!\n");
            }
        }
        else
        {
            m_entity->pos += vel * elapsed;

            // TODO: Fix
            // Check bounds
            /* Rect bounds = scene()->bounds; */
            /* BoxCollider scene_collider(Rect(0.0f, 0.0f, bounds.w, bounds.h)); */
            /* if (!m_collider.Collider::intersects(m_entity->pos, Vec2(bounds.center()), */
            /*             scene_collider)) */
            /* { */
            /*     destroy(); */
            /* } */
        }
    }

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

        return e;
    }
}
