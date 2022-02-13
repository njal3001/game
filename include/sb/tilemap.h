#include <cstddef>
#include <cstdint>

namespace SB
{
    class Tilemap
    {
    private:
        float m_tile_size;
        size_t m_width;
        size_t m_height;

    public:
        struct Mask
        {
            static constexpr uint32_t None = 0;
            static constexpr uint32_t Solid = 1;
        };

        Tilemap(const float tile_size, const size_t width, const size_t height);

        float tile_size() const;
        size_t width() const;
        size_t height() const;
    };
}
