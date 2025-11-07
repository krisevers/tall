#ifndef KERNEL_H
#define KERNEL_H

#include <vector>

#include "expression.h"

namespace tall {

    // @brief a collection of expressions
    struct kernel {
        std::vector<std::pair<variable, expression>> exps;

        kernel(std::initializer_list<std::pair<variable, expression>> exps) : exps(exps) {}

        void record(variable var, expression exp) {
            exps.push_back({var, exp});
        }

        void evaluate() {

        }
    };

}

#endif // KERNEL_H
