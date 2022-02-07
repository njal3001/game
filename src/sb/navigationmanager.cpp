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
        : x(0), y(0), g_val(INFINITY), h_val(INFINITY), parent(nullptr) 
    {}

    float NavigationManager::GridNode::distance(const size_t x, const size_t y)
    {
        return Calc::abs(this->x - x) + Calc::abs(this->y - y);
    }

    NavigationManager::NavigationManager(Scene *scene)
        : m_grid_width((size_t)(scene->bounds.w / tile_size)), 
        m_grid_height((size_t)(scene->bounds.h / tile_size)),
        m_scene(scene)
    {
        m_grid.resize(m_grid_width * m_grid_height);
        for (size_t i = 0; i < m_grid_width * m_grid_height; i++)
        {
            m_grid.emplace_back();
        }
    }

    void NavigationManager::update()
    {
        // Rebuild collider grid
        std::vector<Collider*> colliders;
        m_scene->all(&colliders);

        // Clear grid
        for (size_t i = 0; i < m_grid_width * m_grid_height; i++)
        {
            m_grid[i].clear();
        }

        // Add colliders to grid
        for (auto c : colliders)
        {
            if (!c->is_bounds)
            {
                const size_t i = grid_index(c->pos());
                m_grid[i].push_back(c);
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

                std::unordered_map<size_t, bool> in_open;

                const size_t target_x = pf->target->pos.x / tile_size;
                const size_t target_y = pf->target->pos.y / tile_size;

                // Add starting node
                {
                    const size_t start_x = pf->m_entity->pos.x / tile_size;
                    const size_t start_y = pf->m_entity->pos.y / tile_size;

                    const size_t index = start_x + start_y * m_grid_width;

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
                    in_open[current->x + current->y * m_grid_width] = false;

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

                            if ((x == 0 && y == 0) || x < 0 || x >= (int)m_grid_width || 
                                    y < 0 || y >= (int)m_grid_height)
                            {
                                continue;
                            }


                            GridNode* adj;
                            const size_t adj_index = x + y * m_grid_width;

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

                                //  Add to open queue if not in it already
                                if ((in_open.find(adj_index) != in_open.end()) || 
                                        !in_open[adj_index])
                                {
                                    open.emplace(adj);
                                    in_open[adj_index] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    size_t NavigationManager::grid_index(const Engine::Vec2& pos) const
    {
        const size_t x = pos.x / tile_size;
        const size_t y = pos.y / tile_size;

        return x + y * m_grid_width;
    }

    bool NavigationManager::can_move(const size_t x, const size_t y, 
            const uint32_t avoid_mask) const
    {
        const size_t i = x + y * m_grid_width;

        for (auto c : m_grid[i])
        {
            if (c->mask & avoid_mask) return false;
        }

        return true;
    }
}
