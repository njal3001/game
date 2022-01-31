#include "sb/animator.h"
#include "engine/platform.h"
#include <memory>

namespace SB
{
    using namespace Engine;

    Animator::Animator(const std::string& tex_name)
        : m_tex(nullptr)
    {
        const std::string path = Platform::app_path() + "../res/textures/" + tex_name;
        Image img(path);
        m_tex = std::make_shared<Texture>(img, TextureFormat::RGBA);
    }

    void Animator::render(Renderer* renderer)
    {
        renderer->tex(m_tex, m_entity->pos + offset, Vec2(1, 1), Color::white);
    }
}
