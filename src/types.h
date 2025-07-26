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

    enum dtype {
        INACTIVE,
        VARIABLE,
        PARAMETER,
        COLLECTION,
        SYSTEM,
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
            friend struct registry; // the struct/class allowed to create IDs

    };



    // using primitive = std::variant<
    //     int*,
    //     float*,
    //     double*,
    //     bool*
    // >;

}

#endif // TYPES_H
