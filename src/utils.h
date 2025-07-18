#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <functional>

#include "tall.h"
#include "math.h"

namespace tall {

    // returns true if 'target' is an ancestor (parent, grandparent, etc.) of 'source'
    bool has_parent(const tensor<int>& relations, ID source, ID target) {
        std::size_t n = std::sqrt(relations.size); // assuming square matrix
        std::vector<bool> visited(n, false);
        std::function<bool(ID)> dfs_parent = [&](ID current) -> bool {
            visited[current] = true;
            for (std::size_t parent = 0; parent < n; ++parent) {
                if (relations[parent, current] != 0 && !visited[parent]) {
                    if (parent == target) return true;
                    if (dfs_parent(parent)) return true;
                }
            }
            return false;
        };
        return dfs_parent(source);
    }



    void print(const registry& r) {
        printf("parent -> child : copies:\n");
        for (std::size_t i = 0; i < r.capacity; ++i) {
            for (std::size_t j = 0; j < r.capacity; ++j) {
                if (r.relations[i, j] != 0) {
                    // Print name if available, else index
                    auto print_id = [&](std::size_t idx) {
                        if (!r.names[idx].empty())
                            printf("%s", r.names[idx].c_str());
                        else
                            printf("%zu", idx);
                    };
                    print_id(i);
                    printf(" -> ");
                    print_id(j);
                    printf(" : %d\n", r.relations[i, j]);
                }
            }
        }
    }


}


#endif // UTILS_H