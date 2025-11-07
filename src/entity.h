#ifndef ENTITY_H
#define ENTITY_H

#include <atomic>

namespace tall {

    struct Entity {
        std::size_t value;

        operator std::size_t() const {
            return value;
        }

        Entity(const std::size_t v) : value(v) {}

        private:
            Entity() {
                value = gen();
            }

            static std::size_t gen() {
                static std::atomic<std::size_t> uid{ 0 };
                return uid++;
            }

        public:
            friend struct Registry; // the struct/class allowed to create IDs
    };

}

#endif // ENTITY_H
