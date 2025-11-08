#ifndef ENTITY_H
#define ENTITY_H

#include <atomic>

namespace tall {

    struct Entity {
        std::size_t value;

        operator std::size_t() const {
            return value;
        }

        Entity() {
            value = gen();
        }

        private:
            static std::size_t gen() {
                static std::atomic<std::size_t> uid{ 0 };
                return uid++;
            }

        public:
            // friend struct Registry; // the struct/class allowed to create IDs (temporary public untill Registry is stable)
    };

}

#endif // ENTITY_H
