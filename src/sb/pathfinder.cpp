#include "sb/pathfinder.h"
#include "sb/navigationmanager.h"

namespace SB
{
    using namespace Engine;

    PathFinder::PathFinder()
        : target(nullptr), m_refresh_timer(0.0f), m_path_index(0)
    {}

    void PathFinder::update(const float elapsed)
    {
        const auto manager = scene()->navigation_manager();
        m_refresh_timer -= elapsed;

        // Follow path
        if (m_path.size() > 0 && m_path_index < m_path.size())
        {
            const size_t curr_index = manager->grid_index(m_entity->pos);
            const size_t next_index = manager->grid_index(m_path[m_path_index]);

            // Move towards next tile in path
            if (curr_index == next_index)
            {
                m_path_index++;
            }
        }
    }

    Vec2 PathFinder::move_dir() const
    {
        if (m_path.size() == 0 || m_path_index >= m_path.size())
        {
            return Vec2();
        }

        const Vec2 target = m_path[m_path_index] * scene()->tile_size();
            Vec2(0.5f, 0.5f) * scene()->tile_size();

        return (target - m_entity->pos).norm();
    }
}
