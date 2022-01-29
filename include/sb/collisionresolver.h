#pragma once
#include <stddef.h>

namespace SB
{
    class Scene;

    class CollisionResolver
    {
    private:
        static constexpr size_t resolve_iterations = 3;
        static constexpr float collision_elasticity = 0.01f;
        Scene* m_scene;

    public:
        CollisionResolver(Scene* scene);
        void update();
    };
}
