#ifndef REGISTRY_H
#define REGISTRY_H

#include "types.h"
#include "math.h"

namespace tall {

    struct registry
    {
        std::size_t size;
        std::size_t capacity;
        tall::tensor<dtype> versions;
        tall::tensor<int> relations;
        tall::tensor<std::string> names;

        registry(std::size_t capacity) : capacity(capacity) {
            size = 0;
            versions  = tall::tensor<dtype>(capacity);
            relations = tall::tensor<int>(capacity, capacity);
            names     = tall::tensor<std::string>(capacity);
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

    };

    ID var(registry& r) {
        ID id = r.add(VARIABLE);
        return id;
    }

    ID var(registry& r, std::string name) {
        ID id = var(r);
        r.names[id] = name;
        return id;
    }

    ID param(registry& r) {
        ID id = r.add(PARAMETER);
        return id;
    }

    ID param(registry& r, std::string name) {
        ID id = param(r);
        r.names[id] = name;
        return id;
    }


}

#endif // REGISTRY_H