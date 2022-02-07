#pragma once
#include "engine/maths/vec2.h"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace SB
{
    class Scene;
    class Collider;

    class NavigationManager
    {
    private:
        struct GridNode
        {
            size_t x, y;
            float g_val;
            float h_val;
            const GridNode* parent;

            GridNode();
            float distance(const size_t x, const size_t y);
        };

    public:
        static constexpr float tile_size = 8.0f;

    private:
        static constexpr float refresh_time = 0.5f;

        const size_t m_grid_width;
        const size_t m_grid_height;
        std::vector<std::vector<Collider*>> m_grid;

        Scene *m_scene;

    public:
        NavigationManager(Scene *scene);

        void update();
        size_t grid_index(const Engine::Vec2& pos) const;

    private:
        bool can_move(const size_t x, const size_t y, 
            const uint32_t avoid_mask) const;
    };
}
