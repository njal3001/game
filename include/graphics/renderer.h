#include "texture.h"
#include "maths/vec2.h"
#include "color.h"

namespace Engine
{
    class Renderer
    {
    public:
        virtual void begin() = 0;
        virtual void end() = 0;
        virtual void render() = 0;

        virtual void tex(const Texture& texture, const Vec2& pos, const Color color) = 0;
    };
}
