#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <initializer_list>
#include <memory>
#include <vector>

#include "entity.h"

namespace tall {

    // @brief primitive operations
    enum class Primitive {
        Inactive,
        Add,
        Subtract,
        Multiply,
        Divide,
        Exp,
        Sqrt,
        Pow,
        Negate,
        Variable,       // for leaf nodes
        Parameter,      // for parameters
        Collection,     // for collections
    };

    // @ general struct for capturing primitive types
    struct Node { virtual ~Node() = default; };
    using NodePtr = std::shared_ptr<Node>;

    // @brief represents a collection of variables or other collections.
    struct Collection : public Node {
        const Entity id;

        explicit Collection(Entity id) : id(id) {}
    };

    // @brief represents a variable, holds data and is always a leaf node.
    struct Variable : public Node {
        const Entity id;

        explicit Variable(Entity id) : id(id) {}
    };

    // @brief represents a parameter, holds data are not updatable.
    struct Parameter : public Node {
        const Entity id;

        explicit Parameter(Entity id) : id(id) {}
    };

    // @brief binary operation takes a left-hand and right-hand side.
    struct BinaryOp : public Node {
        NodePtr lhs;
        NodePtr rhs;
        Primitive operation;

        BinaryOp(NodePtr lhs, NodePtr rhs, Primitive op)
            : lhs(std::move(lhs)), rhs(std::move(rhs)), operation(op) {}
    };

    // @brief unary operation takes only a single operand as input.
    struct UnaryOp : public Node {
        NodePtr operand;
        Primitive operation;

        UnaryOp(NodePtr op, Primitive operation)
            : operand(std::move(op)), operation(operation) {}
    };

}   // namespace tall

#endif // PRIMITIVE_H


















#ifdef TEST_PRIMITIVE

using namespace tall;
int main()
{

    Variable x = Variable(new Entity());
    Variable y = Variable(new Entity());

   return 0;
}

#endif // TEST_PRIMITIVE
