#include <vector>
#include <memory>
#include <typeindex>
#include <initializer_list>

#include "../src/types.h"
#include "../src/registry.h"

// component:   holds information on the allocation and type.
// registry:    handles the construction of the model.
// model:       handles the access to data in storage.
// storage:     handles only the allocation and storage of the data.

namespace tall {

    void* allocate(std::size_t dtype_size, std::size_t array_size) {
        return malloc(dtype_size * array_size);
    }

    void deallocate(void* ptr) {
        if (ptr) {
            free(ptr);
            ptr = nullptr;
        }
    }



    struct any {
        std::size_t dtype_size;
        std::size_t array_size;
        std::type_index type_idx;

        any(std::size_t dtype_size, std::size_t array_size, const std::type_info& ti)
            : dtype_size(dtype_size), array_size(array_size), type_idx(ti) {}


        virtual ~any() = default;
    };

    template<typename T>
    struct component : any {
        component(std::size_t size)
            : any(sizeof(T), size, typeid(T)) {}
    };


    struct storage {
        std::vector<std::unique_ptr<void, void(*)(void*)>> ptrs;

        storage() = default;

        storage(int capacity) {
            ptrs.reserve(capacity);
        }
    };

    struct primitive {
        ID x, y, z;
        primitive(ID x, ID y, ID z) : x(x), y(y), z(z) {};
        virtual ~primitive() = default;
    };


    // ID add(registry& r, ID x, ID y, ID z) { return r.add(SYSTEM); }
    // ID sub(registry& r, ID x, ID y, ID z) { return r.add(SYSTEM); }
    // ID mul(registry& r, ID x, ID y, ID z) { return r.add(SYSTEM); }
    // ID div(registry& r, ID x, ID y, ID z) { return r.add(SYSTEM); }


    struct add{
        ID lhs, rhs;
        add(const ID& lhs, const ID& rhs) : lhs(lhs), rhs(rhs) {}
    };

    // Place the operator+ inside an anonymous (detail) namespace to restrict its visibility to this translation unit.
    // This prevents usage outside this file, but not strictly inside kernel construction.
    // For finer control, you could make it a static/private member of kernel, or use a factory function.

    add operator+(const ID& lhs, const ID& rhs) {
        return add(lhs, rhs);
    }

    struct kernel {
        std::vector<primitive>  primitives;
        std::vector<ID>         components;


        kernel(registry& r, std::initializer_list<primitive> p) {
            primitives.reserve(p.size());

            std::size_t idx = 0;
            for (const auto& prim : p) {
                primitives[idx++] = prim;

                tall::log::DEBUG(std::to_string(prim.x));
            }
        }
    };

}

// struct Var {
//     tall::ID id;
//     kernel* k; // pointer to kernel to record ops

//     Var(tall::ID id, kernel* k) : id(id), k(k) {}

//     // Overload +
//     struct AddExpr {
//         Var lhs, rhs;
//         AddExpr(const Var& lhs, const Var& rhs) : lhs(lhs), rhs(rhs) {}
//     };

//     AddExpr operator+(const Var& other) const {
//         return AddExpr(*this, other);
//     }

//     // Overload =
//     Var& operator=(const AddExpr& expr) {
//         k->primitives.push_back(primitive(expr.lhs.id, expr.rhs.id, id));
//         return *this;
//     }
// };



tall::registry r(10);

// variables
auto x = tall::var(r);
auto y = tall::var(r);
auto z = tall::var(r);

// kernel
tall::kernel k(r,
    {
        add(x, y, z),
        mul(x, y, z)
    }
);

int main() {










    // insight: call the functions 'primitives'
    // insight: call the data types 'primitives'

    // insight: after initialization there is no need to get back the data as a result.
    // data-manipulation can be performed by calling specific optimized functions on the data (e.g. view)


    // compute allocation memory requirements
    // iterate through data components and determine the requirements


}
