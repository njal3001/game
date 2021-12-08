#include <vector>
#include "engine/maths/vec2.h"

namespace Engine
{
    struct MassPoint
    {
        Vec2 pos;
        Vec2 vel;
        Vec2 force;
        float mass;
    };

    struct Spring
    {
        MassPoint point1;
        MassPoint point2;
        float stiffnes;
        float rest_length;
        float damp_factor;
    };

    class SoftBody
    {
    private:
        std::vector<MassPoint> m_mass_points;
        std::vector<Spring> m_springs;

    public:
        void update(const float delta_ms);

    private:
        void reset();
        void update_mass_points();
    };
}
