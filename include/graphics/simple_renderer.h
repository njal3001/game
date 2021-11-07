#include <SDL2/SDL.h>
#include "renderer.h"

namespace Engine
{
    class SimpleRenderer : public Renderer
    {
    private:
        SDL_Renderer* m_renderer;

    public:
        SimpleRenderer(SDL_Window* window);
        ~SimpleRenderer();

        void begin() override;
        void end() override;
        void render() override;

        void tex(const Texture& texture, const Vec2& pos, const Color color) override;
    };
}
