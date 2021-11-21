#include <iostream>
#include "platform.h"
#include "input.h"
#include "graphics/renderer.h"

    const std::string default_vert_str =
    "#version 330\n"
    "layout(location=0) in vec2 a_position;\n"
    "layout(location=2) in vec2 a_uv;\n"
    "layout(location=1) in vec4 a_color;\n"
    "out vec2 v_uv;\n"
    "out vec4 v_col;\n"
    "void main(void)\n"
    "{\n"
    "	gl_Position = vec4(a_position.xy, 0, 1);\n"
    "	v_uv = a_uv;\n"
    "	v_col = a_color;\n"
    "}";

    const std::string default_frag_str =
    "#version 330\n"
    "layout(location=0) out vec4 o_col;\n"
    "uniform sampler2D u_texture;\n"
    "uniform int u_use_texture;\n"
    "in vec2 v_uv;\n"
    "in vec4 v_col;\n"
    "void main(void)\n"
    "{\n"
    "   o_col = v_col;\n"
    "   if (u_use_texture > 0)\n"
    "   {\n"
    "       o_col *= texture(u_texture, v_uv);\n"
    "   }\n"
    "}";

int main() 
{
    using namespace Engine;


    if (Platform::init())
    {
        Renderer renderer;

        Image img(Platform::app_path() + "/test_texture.png");
        Texture tex(img);

        while (Platform::update())
        {
            renderer.begin();
            renderer.tex(tex, Vec2(-1.0, -1.0), Vec2(1, 1), Color(255, 255, 255));
            /* renderer.rect(Vec2(-1, -1), Vec2(1, 1), Color(255, 255, 255)); */
            renderer.rect(Vec2(0, 0), Vec2(1, 1), Color(0, 255, 0));
            renderer.end();
            renderer.render();

            Platform::present();
        }
    }

    Platform::shutdown();

}
