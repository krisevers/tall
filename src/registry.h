#ifndef REGISTRY_H
#define REGISTRY_H

#include "tensor.h"
#include "math.h"
#include "types.h"

namespace tall {

    struct registry
    {
        std::size_t size;
        std::size_t capacity;
        tall::tensor<dtype> versions;
        tall::tensor<int> relations;

        registry(std::size_t capacity) : capacity(capacity) {
            size = 0;
            versions  = tall::tensor<dtype>(capacity);
            relations = tall::tensor<int>(capacity, capacity);
        }
        ~registry() {}

        ID add(dtype type) {
            ID newid;
            versions[newid] = type;
            size++;
            return newid;
        }

        bool has(ID id) {
            return id < capacity && versions[id] != INACTIVE;
        }

        expression var() {
            ID id = add(VARIABLE);
            return expression(id);
        }

    };

}

#endif // REGISTRY_H
