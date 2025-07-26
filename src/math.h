#ifndef MATH_H
#define MATH_H

#include <memory>

#include "types.h"

namespace tall {

    enum OPS {
        ADD,
        SUB,
        MUL,
        DIV,
        EXP,
        SQRT,
        VAR,  // For leaf nodes
    };

    struct node { };

    using node_ptr = std::shared_ptr<node>;

    struct variable : public node {
        ID id;

        explicit variable(ID id) : id(id) {}
    };

    struct binary_op : public node {
        node_ptr lhs;
        node_ptr rhs;
        OPS operation;

        binary_op(node_ptr lhs, node_ptr rhs, OPS op)
            : lhs(std::move(lhs)), rhs(std::move(rhs)), operation(op) {}
    };

    struct unary_op : public node {
        node_ptr operand;
        OPS operation;

        unary_op(node_ptr op, OPS operation)
            : operand(std::move(op)), operation(operation) {}
    };

    struct expression {
        private:
            node_ptr root;

        public:
            explicit expression(ID id)
                : root(std::make_shared<variable>(id)) {}

            explicit expression(node_ptr node)
                : root(std::move(node)) {}

            expression(const expression& other) : root(other.root) {}
            expression(expression&& other) noexcept : root(std::move(other.root)) {}

            expression& operator=(const expression& other) {
                if (this != &other) {
                    root = other.root;
                }
                return *this;
            }

            expression& operator=(expression&& other) noexcept {
                if (this != &other) {
                    root = std::move(other.root);
                }
                return *this;
            }

            node_ptr get_root() const { return root; }

            // binary operations
            expression operator+(const expression& other) const {
                auto new_node = std::make_shared<binary_op>(root, other.root, ADD);
                return expression(new_node);
            }
            expression operator-(const expression& other) const {
                auto new_node = std::make_shared<binary_op>(root, other.root, SUB);
                return expression(new_node);
            }
            expression operator*(const expression& other) const {
                auto new_node = std::make_shared<binary_op>(root, other.root, MUL);
                return expression(new_node);
            }
            expression operator/(const expression& other) const {
                auto new_node = std::make_shared<binary_op>(root, other.root, DIV);
                return expression(new_node);
            }
            expression exp(const expression& other) const {
                auto new_node = std::make_shared<unary_op>(root, other.root, EXP);
                return expression(new_node);
            }
            expression sqrt(const expression& other) const {
                auto new_node = std::make_shared<unary_op>(root, other.root, SQRT);
                return expression(new_node);
            }
            // unary operations
            expression operator-() const {
                auto new_node = std::make_shared<unary_op>(root, SUB);
                return expression(new_node);
            }
    };

}

#endif // MATH_H


















#ifdef TEST_MATH

#include "registry.h"

int main()
{

   tall::registry r(10);

   auto x = r.var();
   auto y = r.var();
   auto z = r.var();

   auto expr1 = x + y;

    return 0;
}

#endif // TEST_MATH
