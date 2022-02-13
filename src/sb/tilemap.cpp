#include "sb/tilemap.h"

namespace SB
{
    Tilemap::Tilemap(const float tile_size, const size_t width, const size_t height)
        : m_tile_size(tile_size), m_width(width), m_height(height)
    {}

    float Tilemap::tile_size() const
    {
        return m_tile_size;
    }

    size_t Tilemap::width() const
    {
        return m_width;
    }

    size_t Tilemap::height() const
    {
        return m_height;
    }
}
