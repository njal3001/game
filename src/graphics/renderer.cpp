#include "graphics/renderer.h"
#include <assert.h>
#include <algorithm>
#include <memory>
#include <iostream>
#include "log.h"

namespace Engine
{
    namespace
    {
        const std::string default_vert_str =
		"#version 330\n"
        "uniform mat4 u_matrix;\n"
		"layout(location=0) in vec2 a_position;\n"
		"layout(location=1) in vec2 a_uv;\n"
		"layout(location=2) in vec4 a_color;\n"
		"out vec2 v_uv;\n"
		"out vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = u_matrix * vec4(a_position.xy, 0, 1);\n"
		"	v_uv = a_uv;\n"
		"	v_col = a_color;\n"
		"}";

        const std::string default_frag_str =
		"#version 330\n"
        "uniform sampler2D u_texture;\n"
		"uniform int u_use_texture;\n"
        "layout(location=0) out vec4 o_col;\n"
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

        // TODO: Should not need dummy u_use_texture uniform
        const std::string text_frag_str =
		"#version 330\n"
        "uniform sampler2D u_texture;\n"
        "layout(location=0) out vec4 o_col;\n"
		"uniform int u_use_texture;\n"
		"in vec2 v_uv;\n"
		"in vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
        "   o_col = v_col * texture(u_texture, v_uv).x;\n"
		"}";
    }

    std::shared_ptr<Shader> Renderer::m_default_shader = nullptr;
    std::shared_ptr<Shader> Renderer::m_text_shader = nullptr;

    Renderer::Renderer()
        : m_vertex_count(0), m_vertex_map(nullptr), m_index_map(nullptr), m_matrix(Mat3x3::identity)
    {
        // Create vertex array
        glGenVertexArrays(1, &m_vertex_array);
        glBindVertexArray(m_vertex_array);

        // Create vertex buffer
        glGenBuffers(1, &m_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

        // Specify vertex buffer layout
        size_t pos_offset = 2 * sizeof(GLfloat);
        size_t uv_offset = 2 * sizeof(GLfloat);
        size_t color_offset = 4 * sizeof(GLubyte);

        GLsizei stride = pos_offset + uv_offset + color_offset;

        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)0);

        // texture uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const GLvoid*)pos_offset);

        // color
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (const GLvoid*)(pos_offset + uv_offset));

        // Allocate vertex buffer memory
        glBufferData(GL_ARRAY_BUFFER, RENDERER_MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create index buffer
        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

        // Allocate index buffer memory
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_MAX_INDICES * sizeof(GLushort), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        m_matrix_stack.push_back(Mat3x3::identity);

        // Create shaders
        if (!m_default_shader)
        {
            m_default_shader = std::make_shared<Shader>(default_vert_str, default_frag_str);
        }

        if (!m_text_shader)
        {
            m_text_shader = std::make_shared<Shader>(default_vert_str, text_frag_str);
        }
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_vertex_buffer);
        glDeleteBuffers(1, &m_index_buffer);
        glDeleteVertexArrays(1, &m_vertex_array);
    }

    void Renderer::push_matrix(const Mat3x3& matrix, const bool absolute)
    {
        if (absolute)
        {
            m_matrix = matrix;
        }
        else
        {
            m_matrix = m_matrix * matrix;
        }

        m_matrix_stack.push_back(matrix);
    }

    Mat3x3 Renderer::pop_matrix()
    {
        assert(m_matrix_stack.size() > 1);

        Mat3x3 top = m_matrix_stack.back();
        m_matrix_stack.pop_back();
        m_matrix = m_matrix_stack.back();

        return top;
    }

    Mat3x3 Renderer::peek_matrix()
    {
        return m_matrix;
    }


    void Renderer::push_shader(const std::shared_ptr<Shader>& shader)
    {
        m_shader_stack.push_back(shader);
        m_shader = shader;
    }

    std::shared_ptr<Shader> Renderer::peek_shader() const
    {
        return m_shader;
    }

    std::shared_ptr<Shader> Renderer::pop_shader()
    {
        assert(m_shader_stack.size() > 0);

        std::shared_ptr<Shader> back = m_shader_stack.back();
        m_shader_stack.pop_back();

        if (m_shader_stack.size() > 0)
        {
            m_shader = m_shader_stack.back();
        }
        else
        {
            m_shader = nullptr;
        }

        return back;
    }

    void Renderer::make_vertex(float px, float py, float tx, float ty, Color color)
    {
        m_vertex_map->pos.x = m_matrix.m11 * px + m_matrix.m21 * py + m_matrix.m31;
        m_vertex_map->pos.y = m_matrix.m12 * px + m_matrix.m22 * py + m_matrix.m32;
        m_vertex_map->uv.x = tx;
        m_vertex_map->uv.y = ty;
        m_vertex_map->color = color;

        m_vertex_map++;
    }


    void Renderer::update_batch(unsigned int count, unsigned int texture)
    {
        if (m_batches.empty() || m_batches.back().texture != texture ||m_batches.back().shader != m_shader)
        {
            m_batches.push_back(
            {
                .count = 0,
                .texture = texture,
                .shader = m_shader,
            });
        }

        m_batches.back().count += count;
    }

    void Renderer::push_triangle(float px0, float py0, float px1, float py1,
             float px2, float py2, unsigned int tex, float tx0,
             float ty0, float tx1, float ty1, float tx2, float ty2, 
             Color c0, Color c1, Color c2)
    {
        make_vertex(px0, py0, tx0, ty0, c0);
        make_vertex(px1, py1, tx1, ty1, c1);
        make_vertex(px2, py2, tx2, ty2, c2);

        *m_index_map = m_vertex_count;
        m_index_map++;
        *m_index_map = m_vertex_count + 1;
        m_index_map++;
        *m_index_map = m_vertex_count + 2;
        m_index_map++;

        update_batch(3, tex);
        m_vertex_count += 3;
    }

    void Renderer::push_quad(float px0, float py0, float px1, float py1,
            float px2, float py2, float px3, float py3,
            unsigned int tex, float tx0, float ty0, float tx1, 
            float ty1, float tx2, float ty2, float tx3, float ty3, 
            Color c0, Color c1, Color c2, Color c3)
    {
        make_vertex(px0, py0, tx0, ty0, c0);
        make_vertex(px1, py1, tx1, ty1, c1);
        make_vertex(px2, py2, tx2, ty2, c2);
        make_vertex(px3, py3, tx3, ty3, c3);

        *m_index_map = m_vertex_count;
        m_index_map++;
        *m_index_map = m_vertex_count + 1;
        m_index_map++;
        *m_index_map = m_vertex_count + 2;
        m_index_map++;

        *m_index_map = m_vertex_count + 2;
        m_index_map++;
        *m_index_map = m_vertex_count + 3;
        m_index_map++;
        *m_index_map = m_vertex_count;
        m_index_map++;

        update_batch(6, tex);
        m_vertex_count += 4;
    }

    void Renderer::begin()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        m_vertex_map = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        m_index_map = (GLushort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Renderer::tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color)
    {
        assert(m_vertex_map && m_index_map);

        push_triangle(pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y, 0, 0, 0, 0, 0, 0, 0, 
                color, color, color);
    }

    void Renderer::rect(const Vec2& pos, const Vec2& size, const Color color)
    {
        assert(m_vertex_map && m_index_map);

        push_quad(pos.x, pos.y, pos.x, pos.y + size.y, pos.x + size.x, pos.y + size.y, 
                pos.x + size.x, pos.y, 0, 0, 0, 0, 0, 0, 0, 0, 0, color, color, color, color);
    }

    void Renderer::tex(const std::shared_ptr<Texture>& texture, const Vec2& pos, const Color color)
    {
        assert(m_vertex_map && m_index_map);
        
        int w = texture->width();
        int h = texture->height();

        push_quad(pos.x, pos.y, pos.x, pos.y + h, pos.x + w, pos.y + h, pos.x + w, pos.y,
                texture->id(), 0, 1, 0, 0, 1, 0, 1, 1, color, color, color, color);
    }

    void Renderer::tex(const std::shared_ptr<Texture>& texture, const Vec2& pos, const Vec2& scale, const Color color)
    {
        push_matrix(Mat3x3::create_scale(scale));
        tex(texture, pos, color);
        pop_matrix();
    }

    void Renderer::tex(const Subtexture& sub, const Vec2& pos, const Color color)
    {
        assert(m_vertex_map && m_index_map);
        
        Rect src = sub.source();
        std::array<Vec2, 4> tex_coords = sub.tex_coords();

        push_quad(pos.x, pos.y, pos.x, pos.y + src.h, pos.x + src.w, pos.y + src.h, 
                pos.x + src.w, pos.y, sub.texture_ref()->id(), 
                tex_coords[0].x, 1.0f - tex_coords[0].y, tex_coords[1].x, 1.0f - tex_coords[1].y, 
                tex_coords[2].x, 1.0f - tex_coords[2].y, tex_coords[3].x, 1.0f - tex_coords[3].y, 
                color, color, color, color);
    }


    void Renderer::tex(const Subtexture& sub, const Vec2& pos, const Vec2& scale, const Color color)
    {
        push_matrix(Mat3x3::create_scale(scale));
        tex(sub, pos, color);
        pop_matrix();
    }


    void Renderer::str(const Font& font, const std::string& text, const Vec2& pos, const Color color)
    {
        assert(m_vertex_map && m_index_map);

        push_shader(m_text_shader);

        Vec2 ch_pos = pos;

        const unsigned int height = font.height();

        for (unsigned int i = 0; i < text.size(); i++)
        {
            if (text[i] == '\n')
            {
                ch_pos.y -= height;
                ch_pos.x = pos.x;
                continue;
            }

            const char ch = text.at(i);

            const Character& character = font.get_character((unsigned char)ch);

            tex(character.subtexture, ch_pos + character.offset, color);

            ch_pos.x += character.advance;
        }

        pop_shader();
    }

    void Renderer::str(const Font& font, const std::string& text, const Vec2& pos, const Vec2& scale, const Color color)
    {
        push_matrix(Mat3x3::create_scale(scale));
        str(font, text, pos, color);
        pop_matrix();
    }

    void Renderer::end()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        m_vertex_map = nullptr;
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        m_index_map = nullptr;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Renderer::render(const Mat4x4& matrix)
    {
        assert(!m_vertex_map && !m_index_map);

        glBindVertexArray(m_vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);


        // Render batches
        unsigned int offset = 0;
        for (auto& batch : m_batches)
        {
            if (!batch.shader)
            {
                batch.shader = m_default_shader;
            }

            glUseProgram(batch.shader->id());

            int u_use_texture = 0;
            // Bind texture if it has one
            if (batch.texture > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, batch.texture);
                u_use_texture = 1;
            }

            // TODO: Add support for multiple textures?
            batch.shader->set_uniform_mat4("u_matrix", matrix);
            batch.shader->set_uniform_1i("u_texture", 0);
            batch.shader->set_uniform_1i("u_use_texture", u_use_texture);

            glDrawElements(GL_TRIANGLES, batch.count, GL_UNSIGNED_SHORT, (void*)(offset * sizeof(GLushort)));

            offset += batch.count;
        }

        glUseProgram(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_vertex_count = 0;
        m_batches.clear();
    }
}
