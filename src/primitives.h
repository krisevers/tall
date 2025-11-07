#ifndef MATH_H
#define MATH_H

#include <initializer_list>
#include <memory>
#include <vector>

#include "types.h"

namespace tall {

    // @brief primitive operations
    enum OPS {
        INACTIVE,
        ADD,
        SUB,
        MUL,
        DIV,
        EXP,
        SQR,
        POW,
        VAR,  // for leaf nodes
        PAR,  // for parameters
        COL,  // for collections
    };

    // @ general struct for capturing primitive types
    struct node { virtual ~node() = default; };
    using node_ptr = std::shared_ptr<node>;

    // @brief represents a collection of variables or other collections.
    struct collection : public node {
        ID id;

        explicit collection(ID id) : id(id) {}
    };

    // @brief represents a variable, holds data and is always a leaf node.
    struct variable : public node {
        ID id;

        explicit variable(ID id) : id(id) {}
    };

    // @brief represents a parameter, holds data are not updatable.
    struct parameter : public node {
        ID id;

        explicit parameter(ID id) : id(id) {}
    };

    // @brief binary operation takes a left-hand and right-hand side.
    struct binary_op : public node {
        node_ptr lhs;
        node_ptr rhs;
        OPS operation;

        binary_op(node_ptr lhs, node_ptr rhs, OPS op)
            : lhs(std::move(lhs)), rhs(std::move(rhs)), operation(op) {}
    };

    // @brief unary operation takes only a single operand as input.
    struct unary_op : public node {
        node_ptr operand;
        OPS operation;

        unary_op(node_ptr op, OPS operation)
            : operand(std::move(op)), operation(operation) {}
    };

}   // namespace tall

#endif // MATH_H


















#ifdef TEST_MATH

#include "registry.h"

int main()
{

   tall::registry r(10);

   auto x = r.var();
   auto y = r.var();
   auto z = r.var();
   auto sigma = r.var();
   auto beta  = r.var();
   auto rho   = r.var();

   auto dx = sigma*(y-x);
   auto dy = x*(rho-z)-y;
   auto dz = x*y - beta*y;

   return 0;
}

#endif // TEST_MATH
