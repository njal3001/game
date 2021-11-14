#include "graphics/renderer.h"
#include <assert.h>
#include <algorithm>
#include <memory>
#include "log.h"
#include <iostream>

namespace Engine
{
    namespace
    {
        const std::string default_vert_str =
		"#version 330\n"
		"layout(location=0) in vec2 a_position;\n"
		"layout(location=1) in vec2 a_uv;\n"
		"layout(location=2) in vec4 a_color;\n"
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
        "layout (location = 0) out vec4 o_col;\n"
		"uniform sampler2D u_texture;\n"
		"in vec2 v_uv;\n"
		"in vec4 v_col;\n"
		"void main(void)\n"
		"{\n"
        "   o_col = vec4(1, 1, 1, 1);\n"
        /* "   if (u_texture)\n" */
        /* "   {\n" */
        /* "       o_col *= texture(u_texture, v_uv);\n" */
        /* "   }\n" */
		"}";
    }

    std::shared_ptr<Shader> Renderer::m_default_shader = nullptr;

    Renderer::Renderer()
        :  m_vert_index(0), m_ind_p(m_indices)
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
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, stride, (const GLvoid*)(pos_offset + uv_offset));

        // Allocate vertex buffer memory
        glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create index buffer
        glGenBuffers(1, &m_index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        
        // Allocate index buffer memory
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_INDICIES_SIZE * sizeof(GLushort), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    Renderer::~Renderer()
    {
        glDeleteBuffers(1, &m_vertex_buffer);
        glDeleteBuffers(1, &m_index_buffer);
        glDeleteVertexArrays(1, &m_vertex_array);
    }

    void Renderer::make_vertex(float px, float py, int tex, float tx, float ty, Color color)
    {
        Vertex* vert_p = &m_vertices[m_vert_index];

        vert_p->pos.x = px;
        vert_p->pos.y = py;
        vert_p->uv.x = tx;
        vert_p->uv.y = ty;
        vert_p->color = color;

        m_textures[m_vert_index] = tex;
        m_vert_index++;
    }

    void Renderer::push_triangle(float px0, float py0, float px1, float py1,
             float px2, float py2, int tex, float tx0, float ty0, 
             float tx1, float ty1, float tx2, float ty2, 
             Color c0, Color c1, Color c2)
    {
        make_vertex(px0, py0, tex, tx0, ty0, c0);
        make_vertex(px1, py1, tex, tx1, ty1, c1);
        make_vertex(px2, py2, tex, tx2, ty2, c2);

        *m_ind_p = m_vertex_count;
        m_ind_p++;
        *m_ind_p = m_vertex_count + 1;
        m_ind_p++;
        *m_ind_p = m_vertex_count + 2;
        m_ind_p++;

        m_index_count += 3;
        m_vertex_count += 3;
        m_texture_count[tex] += 3;
    }

    void Renderer::push_quad(float px0, float py0, float px1, float py1,
            int tex, float tx0, float ty0, float tx1, float ty1, 
            float tx2, float ty2, float tx3, float ty3, 
            Color c0, Color c1, Color c2, Color c3)
    {
        make_vertex(px0, py0, tex, tx0, ty0, c0);
        make_vertex(px0, py1, tex, tx1, ty1, c1);
        make_vertex(px1, py1, tex, tx2, ty2, c2);
        make_vertex(px1, py0, tex, tx3, ty3, c3);

        *m_ind_p = m_vertex_count;
        m_ind_p++;
        *m_ind_p = m_vertex_count + 1;
        m_ind_p++;
        *m_ind_p = m_vertex_count + 2;
        m_ind_p++;

        *m_ind_p = m_vertex_count + 2;
        m_ind_p++;
        *m_ind_p = m_vertex_count + 3;
        m_ind_p++;
        *m_ind_p = m_vertex_count;

        m_index_count += 6;
        m_vertex_count += 4;
        m_texture_count[tex] += 6;
    }

    void Renderer::tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Color color)
    {
        push_triangle(pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y, 0, 0, 0, 0, 0, 0, 0, 
                color, color, color);

    }

    void Renderer::rect(const Vec2& pos, const Vec2& size, const Color color)
    {
        push_quad(pos.x, pos.y, pos.x + size.x, pos.y + size.y, 0,
                0, 0, 0, 0, 0, 0, 0, 0, color, color, color, color);
    }

    void Renderer::tex(const Texture& texture, const Vec2& pos, const Color color)
    {
        int w = texture.width();
        int h = texture.height();

        // TODO: Uvs are probably wrong
        push_quad(pos.x, pos.y, pos.x + w, pos.y + h, texture.id(),
                0, 0, 0, 1, 1, 1, 1, 0, color, color, color, color);
    }

    void Renderer::render()
    {
        // Sorting indices by texture
        std::sort(std::begin(m_indices), std::begin(m_indices) + m_index_count, 
                [this](const GLushort a, const GLushort b) -> bool
                {
                   return m_textures[a] < m_textures[b] || (m_textures[a] == m_textures[b] && a < b); 
                });
        
        // Upload vertices and indices
        glBindVertexArray(m_vertex_array);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertex_count * sizeof(Vertex), m_vertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_index_count * sizeof(GLushort), m_indices);

        // Create default shader if it's not created yet
        if (!m_default_shader)
        {
            m_default_shader = std::make_shared<Shader>(default_vert_str, default_frag_str);
        }

        glUseProgram(m_default_shader->id());

        // Perform render passes
        unsigned int offset = 0;
        for (auto& tex : m_texture_count)
        {
            // Check if nothing to draw
            if (!tex.second)
            {
                continue;
            }


            int u_texture = -1;
            // Bind texture if it has one
            if (tex.first > 0)
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, tex.first);
                u_texture = 0;
            }

            m_default_shader->set_uniform_1i("u_texture", u_texture);

            glDrawElements(GL_TRIANGLES, tex.second, GL_UNSIGNED_SHORT, (void*)(offset * sizeof(GLushort)));

            offset += tex.second;
        }

        glUseProgram(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Reset for next render
        m_vert_index = 0;
        m_ind_p = m_indices;

        for (auto& tex : m_texture_count)
        {
            tex.second = 0;
        }

        m_index_count = 0;
        m_vertex_count = 0;
    }
}
