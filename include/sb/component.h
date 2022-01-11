#include <stdint.h>

namespace SB
{
    class Entity;

    class Component
    {
    // TODO: Duplicate, same as in entity
    private:
        uint8_t m_type;

        class Types
        {
            private:
                inline static uint8_t s_counter = 0;

            public:
                template<class T>
                static uint8_t id()
                {
                    static const uint8_t val = s_counter++;
                    return val;
                }
        };
        
    protected:
        Entity* m_entity;
        Component* m_next;
        Component* m_prev;

    public:
        Component(Entity* entity);

        uint8_t type() const;

        Entity* entity() const;

        Component* next() const;
        Component* prev() const;

        void destroy();
    };
}
