#ifndef REGISTRY_H
#define REGISTRY_H

#include <initializer_list>

#include "tensor.h"
#include "types.h"
#include "expression.h"
#include "primitives.h"
#include "utils.h"

namespace tall {

    struct registry
    {
        std::size_t size;
        std::size_t capacity;
        tall::tensor<OPS> versions;
        tall::tensor<int> relations;

        registry(std::size_t capacity);
        ~registry();

        ID add(OPS type);

        bool has(ID id);

        node var();
        node par();

        RESULT give(ID parent, ID child, int copies);
        node assemble(std::initializer_list<node> components, std::size_t copies=1);

    };

}

#endif // REGISTRY_H
