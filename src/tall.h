#ifndef TALL_H
#define TALL_H

#include <iostream>
#include <initializer_list>
#include <string>
#include <functional>
#include <cmath>

#include "types.h"
#include "registry.h"
#include "model.h"
#include "math.h"
#include "utils.h"
#include "logger.h"


namespace tall {

    RESULT give(registry& r, ID parent, ID child, int copies) {
        if (!r.has(parent)) {
            tall::log::ERROR("parent not initialized...\n");
            return FAILURE;
        }
        if (!r.has(child)) {
            tall::log::ERROR("child not initialized...\n");
            return FAILURE;
        }
        if (has_parent(r.relations, parent, child)) {
            tall::log::ERROR("child is an ancestor of parent...\n");
            return FAILURE;
        }
        if (parent == child) {
            tall::log::ERROR("not allowed to make circular hierarchies...\n");
            return FAILURE;
        }

        r.relations[parent, child] = copies;

        return SUCCESS;
    }

    ID assemble(registry& r, std::initializer_list<ID> components, std::size_t copies=1) {
        ID p = r.add(COLLECTION);
        for (auto c : components) {
            give(r, p, c, copies);
        }
        return p;
    }




    RESULT allocate(const registry r, model& m) {
        // the dense arrays are created
        for (std::size_t i; i<r.size; i++) {
        
        }
        // the view graph is created
        // tall::view s = data.slice({1}, {3})


        return SUCCESS;
    }
}


#endif // TALL_H