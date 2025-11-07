#ifndef TYPES_H
#define TYPES_H

#include <atomic>
#include <typeinfo>
#include <typeindex>
#include <complex>
#include <variant>

namespace tall {

    enum RESULT {
        FAILURE,
        SUCCESS,
    };

    struct ID {
        std::size_t value;

        operator std::size_t() const {
            return value;
        }

        ID(const std::size_t v) : value(v) {}

        private:
            ID() {
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

#endif // TYPES_H
