#include "engine/graphics/renderer.h"
#include "sb/ecs.h"
#include <memory>

namespace SB
{
    // TODO: Implement with texture atlas system
    class Animator : public Component
    {
    private:
        std::shared_ptr<Engine::Texture> m_tex;

    public:
        float rotation;
        Engine::Vec2 offset;

    public:
        Animator(const std::string& tex_name);

        void render(Engine::Renderer* renderer) override;
    };
}
