#pragma once
#include "engine/maths/vec2.h"
#include <stddef.h>
#include <vector>

namespace SB
{
    class Scene;

    class CollisionManager
    {
    private:
        static constexpr size_t resolve_iterations = 3;
        static constexpr float collision_elasticity = 0.01f;

        Scene *m_scene;

    public:
        CollisionManager(Scene *scene);

        void update();
    };
}
