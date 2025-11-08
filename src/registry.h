#ifndef REGISTRY_H
#define REGISTRY_H

#include <initializer_list>

#include "tensor.h"
#include "types.h"
#include "expression.h"
#include "primitive.h"
#include "utils.h"

namespace tall {

    struct Registry
    {
        std::size_t size;
        std::size_t capacity;
        tall::tensor<Operation> versions;
        tall::tensor<int> relations;

        registry(std::size_t capacity);
        ~registry();

        Entity add(Operation type);

        bool has(Entity id);

        Node variable();
        Node parameter();

        Result give(Entity parent, Entity child, int copies);
        Node assemble(std::initializer_list<Node> components, std::size_t copies=1);

    };

}

#endif // REGISTRY_H
