#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "primitives.h"

namespace tall {

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
            // unary operations
            expression operator-() const {
                auto new_node = std::make_shared<unary_op>(root, SUB);
                return expression(new_node);
            }
    };

    expression exp(const expression& other) {
        auto new_node = std::make_shared<unary_op>(other.get_root(), EXP);
        return expression(new_node);
    }
    expression sqrt(const expression& other) {
        auto new_node = std::make_shared<unary_op>(other.get_root(), SQR);
        return expression(new_node);
    }
    expression pow(const expression& base, const double exp) {
        auto new_node = std::make_shared<unary_op>(base.get_root(), POW);
        return expression(new_node);
    }
}

#endif // EXPRESSION_H
