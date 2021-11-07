#include "texture.h"

namespace Engine
{
    class Renderer
    {
    public:
        void begin();
        void push(const Texture& texture);
        void end();
        void render();

    };
}
