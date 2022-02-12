#include "sb/navigationmanager.h"
#include <cmath>
#include <queue>
#include <unordered_map>
#include "sb/pathfinder.h"
#include "engine/maths/calc.h"
#include "sb/ecs.h"
#include "sb/collider.h"

namespace SB
{
    using namespace Engine;

    NavigationManager::GridNode::GridNode()
        : x(0), y(0), g_val(INFINITY), h_val(INFINITY), parent(nullptr),
        state(NavigationManager::GridNode::State::UNEXPLORED)
    {}

    float NavigationManager::GridNode::distance(const size_t x, const size_t y)
    {
        return Calc::abs(this->x - x) + Calc::abs(this->y - y);
    }

    NavigationManager::NavigationManager(Scene *scene)
        : m_scene(scene)
    {
        m_grid.resize(scene->width() * scene->height());
        for (size_t i = 0; i < scene->width() * scene->height(); i++)
        {
            m_grid.emplace_back();
        }
    }

    void NavigationManager::update()
    {
        // TODO: Unecessary to rebuild every frame
        // Rebuild collider grid
        std::vector<Collider*> solids;
        m_scene->all(&solids, Mask::Solid);

        // Clear grid
        for (size_t i = 0; i < m_scene->width() * m_scene->height(); i++)
        {
            m_grid[i] = false;
        }

        // Mark solid tiles in grid
        for (auto c : solids)
        {
            const Rect bounds = c->bounding_box();

            const size_t start_x = bounds.x / m_scene->tile_size();
            const size_t end_x = (bounds.x + bounds.w) / m_scene->tile_size();
            const size_t start_y = bounds.y / m_scene->tile_size();
            const size_t end_y = (bounds.y + bounds.h) / m_scene->tile_size();

            if (!in_bound(start_x, start_y) || !in_bound(end_x, end_y)) continue;

            for (size_t x = start_x; x <= end_x; x++)
            {
                for (size_t y = start_y; y <= end_y; y++)
                {
                    printf("tile: (%d, %d)\n", (int)x, (int)y);
                    const size_t index = grid_index(x, y);
                    m_grid[index] = true;
                }
            }
        }

        // Calculate path for path finders
        std::vector<PathFinder*> pathfinders;
        m_scene->all(&pathfinders);

        auto compare = [](GridNode *n1, GridNode *n2)
        {
            return n1->g_val + n1->h_val <= n2->g_val + n2->h_val;
        };

        for (auto pf : pathfinders)
        {
            if (pf->m_refresh_timer <= 0.0f && pf->target)
            {
                // Find A* path
                pf->m_refresh_timer = refresh_time;
                pf->m_path_index = 0;
                pf->m_path.clear();

                std::unordered_map<size_t, GridNode> nodes;

                std::priority_queue<GridNode*, std::vector<GridNode*>,
                    decltype(compare)> open(compare);

                const size_t target_x = pf->target->pos.x / m_scene->tile_size();
                const size_t target_y = pf->target->pos.y / m_scene->tile_size();

                // Add starting node
                {
                    const size_t start_x = pf->m_entity->pos.x / m_scene->tile_size();
                    const size_t start_y = pf->m_entity->pos.y / m_scene->tile_size();

                    const size_t index = grid_index(start_x, start_y);

                    nodes[index] = GridNode();

                    auto start = &nodes[index];
                    start->x = start_x;
                    start->y = start_y;
                    start->g_val = 0.0f;
                    start->h_val = start->distance(target_x, target_y);

                    open.emplace(start);
                }

                while (!open.empty())
                {
                    GridNode* current = open.top();
                    open.pop();
                    current->state = GridNode::State::EXPLORED;

                    // Check if reached target
                    if (current->x == target_x && current->y == target_y)
                    {
                        // Retrace path, does not include start tile
                        const GridNode *p = current;
                        while (p->parent)
                        {
                            pf->m_path.push_back(Vec2((float)p->x, (float)p->y));
                            p = p->parent;
                        }

                        break;
                    }

                    // Look at all adjacent tiles
                    for (int dy = -1; dy < 2; dy++)
                    {
                        for (int dx = -1; dx < 2; dx++)
                        {
                            const int x = current->x + dx;
                            const int y  = current->y + dy;

                            if (x == 0 && y == 0) continue;
                            if (!can_move(x, y)) continue;

                            GridNode* adj;
                            const size_t adj_index = grid_index(x, y);

                            // Check if adjacent has been visited before
                            if (nodes.find(adj_index) == nodes.end())
                            {
                                nodes[adj_index] = GridNode();
                                adj = &nodes[adj_index];

                                // Intitialize node
                                adj->x = x;
                                adj->y = y;
                                adj->h_val = current->distance(target_x, target_y);
                            }
                            else
                            {
                                adj = &nodes[adj_index];
                            }

                            // Found better path to adjacent
                            if (adj->g_val > current->g_val + 1)
                            {
                                adj->g_val = current->g_val + 1;
                                adj->parent = current;

                                //  Add to open queue if not explored yet
                                if (adj->state == GridNode::State::UNEXPLORED)
                                {
                                    open.emplace(adj);
                                    adj->state = GridNode::State::OPEN;
                                }
                                else if (adj->state == GridNode::State::EXPLORED)
                                {
                                    // Adjacent node has already been explored, propagate
                                    // path improvement
                                    prop_path(&nodes, adj);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void NavigationManager::prop_path(std::unordered_map<size_t, GridNode> *nodes,
                const GridNode *parent)
    {
        for (int dy = -1; dy < 2; dy++)
        {
            for (int dx = -1; dx < 2; dx++)
            {
                const int x = parent->x + dx;
                const int y = parent->y + dy;

                if (x == 0 || y == 0) continue;
                if (!in_bound(x, y)) continue;

                const size_t i = grid_index(x, y);
                if (nodes->find(i) != nodes->end())
                {
                    GridNode *child = &(*nodes)[i];
                    if (parent->g_val + 1 < child->g_val)
                    {
                        child->g_val = parent->g_val + 1;
                        child->parent = parent;
                        prop_path(nodes, child);
                    }
                }
            }
        }
    }

    size_t NavigationManager::grid_index(const size_t x, const size_t y) const
    {
        return x + y * m_scene->width();
    }

    size_t NavigationManager::grid_index(const Engine::Vec2& pos) const
    {
        const size_t x = pos.x / m_scene->tile_size();
        const size_t y = pos.y / m_scene->tile_size();

        return grid_index(x, y);
    }

    bool NavigationManager::in_bound(const int x, const int y) const
    {
        return x >= 0 && x < (int)m_scene->width() && y >= 0 && y < (int)m_scene->height();
    }

    bool NavigationManager::can_move(const int x, const int y) const
    {
        if (!in_bound(x, y)) return false;

        const size_t i = grid_index(x, y);

        return !m_grid[i];
    }

    void NavigationManager::render_grid(Renderer* renderer)
    {
        std::vector<Collider*> solids;
        m_scene->all(&solids, Mask::Solid);

        // Mark solid tiles in grid
        for (auto c : solids)
        {
            const Rect bounds = c->bounding_box();

            const size_t start_x = bounds.x / m_scene->tile_size();
            const size_t end_x = (bounds.x + bounds.w) / m_scene->tile_size();
            const size_t start_y = bounds.y / m_scene->tile_size();
            const size_t end_y = (bounds.y + bounds.h) / m_scene->tile_size();

            if (!in_bound(start_x, start_y) || !in_bound(end_x, end_y)) continue;

            for (size_t x = start_x; x <= end_x; x++)
            {
                for (size_t y = start_y; y <= end_y; y++)
                {
                    const Rect r(x * m_scene->tile_size(), y * m_scene->tile_size(), m_scene->tile_size(), m_scene->tile_size());
                    renderer->rect(r, Color::white);
                }
            }
        }

        /* bool draw = true; */
        /* for (size_t x = 0; x < m_scene->width(); x++) */
        /* { */
        /*     for (size_t y = 0; y < m_scene->width(); y++) */
        /*     { */
        /*         if (draw) */
        /*         { */
        /*             const Rect r(x * m_scene->tile_size(), y * m_scene->tile_size(), m_scene->tile_size(), m_scene->tile_size()); */
        /*             renderer->rect(r, Color::white); */
        /*         } */

        /*         draw = !draw; */
        /*     } */

        /* } */
    }
}
