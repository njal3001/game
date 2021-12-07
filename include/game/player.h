#include <vector>
#include "engine/maths/vec2.h"
#include "engine/graphics/renderer.h"

namespace Game
{
    class Player
    {
    private:
        
        struct Point
        {
            Engine::Vec2 pos;
        };

        struct Spring
        {
            Point point1;
            Point point2;
            float rest_length;
        };

        std::vector<Spring> m_springs;

    public:
        void update(float delta_ms);
        void render(Engine::Renderer& renderer);

    };
}
