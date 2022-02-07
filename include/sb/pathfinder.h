#include "engine/maths/vec2.h"
#include "sb/ecs.h"
#include <vector>

namespace SB
{
    class PathFinder : public Component
    {

    friend class NavigationManager;

    public:
        Entity* target;
        uint32_t avoid_mask;

    private:
        float m_refresh_timer;
        std::vector<Engine::Vec2> m_path;
        size_t m_path_index;

    public:
        PathFinder();

        void update(const float elapsed) override;

        Engine::Vec2 move_dir() const;
    };
}
