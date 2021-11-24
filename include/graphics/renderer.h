#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include "maths/vec2.h"
#include "maths/mat4x4.h"
#include "maths/mat3x3.h"
#include "graphics/texture.h"
#include "graphics/color.h"
#include "graphics/shader.h"
#include "graphics/subtexture.h"

namespace Engine
{

#define RENDERER_MAX_SPRITES 10000
#define RENDERER_MAX_VERTICES RENDERER_MAX_SPRITES * 4
#define RENDERER_MAX_INDICES RENDERER_MAX_SPRITES * 6

    // TODO: Add custom shader support. Create mesh struct?
    class Renderer
    {
    private:
        struct Vertex
        {
            Vec2 pos;
            Vec2 uv;
            Color color;
        };

        struct RenderPass
        {
            unsigned int texture;
            unsigned int count;
        };

        GLuint m_vertex_array;
        GLuint m_vertex_buffer;
        GLuint m_index_buffer;

        unsigned int m_vertex_count;
        Vertex* m_vertex_map;
        GLushort* m_index_map;
        std::vector<RenderPass> m_render_passes;

        Mat3x3 m_matrix;
        std::vector<Mat3x3> m_matrix_stack;

        static std::shared_ptr<Shader> m_default_shader;

    public:
        Renderer();
        ~Renderer();

        Renderer(const Renderer& other) = delete;
        Renderer& operator=(const Renderer& other) = delete;

        void push_matrix(const Mat3x3& matrix, bool absolute);
        Mat3x3 pop_matrix();
        Mat3x3 peek_matrix();

        void begin();
        void render(const Mat4x4& matrix);
        void end();

        void rect(const Vec2& pos, const Vec2& size, const Color color);
        void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color); 
        void tex(const std::shared_ptr<Texture>& texture, const Vec2& pos, const Color color);
        void tex(const std::shared_ptr<Texture>& texture, const Vec2& pos, const Vec2& scale, const Color color);
        void tex(const Subtexture& sub, const Vec2& pos, const Color color);
        void tex(const Subtexture& sub, const Vec2& pos, const Vec2& scale, const Color color);

    private:
        void make_vertex(float px, float py, float tx, float ty, Color color);

        void push_triangle(float px0, float py0, float px1, float py1,
                 float px2, float py2, unsigned int tex, float tx0,
                 float ty0, float tx1, float ty1, float tx2, float ty2, 
                 Color c0, Color c1, Color c2);

        void push_quad(float px0, float py0, float px1, float py1,
                float px2, float py2, float px3, float py3,
                unsigned int tex, float tx0, float ty0, float tx1, 
                float ty1, float tx2, float ty2, float tx3, float ty3, 
                Color c0, Color c1, Color c2, Color c3);

        void update_render_pass(unsigned int count, unsigned int texture);
    };
}
