#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "primitive.h"

namespace tall {

    struct Expression {
        private:
            NodePtr root;

        public:
            explicit Expression(Entity entity)
                : root(std::make_shared<Variable>(entity)) {}

            explicit Expression(NodePtr node)
                : root(std::move(node)) {}

            Expression(const Expression& other) : root(other.root) {}
            Expression(Expression&& other) noexcept : root(std::move(other.root)) {}

            Expression& operator=(const Expression& other) {
                if (this != &other) {
                    root = other.root;
                }
                return *this;
            }

            Expression& operator=(Expression&& other) noexcept {
                if (this != &other) {
                    root = std::move(other.root);
                }
                return *this;
            }

            NodePtr get_root() const { return root; }

            // binary operations
            Expression operator+(const Expression& other) const {
                auto new_node = std::make_shared<BinaryOp>(root, other.root, Primitive::Add);
                return Expression(new_node);
            }
            Expression operator-(const Expression& other) const {
                auto new_node = std::make_shared<BinaryOp>(root, other.root, Primitive::Subtract);
                return Expression(new_node);
            }
            Expression operator*(const Expression& other) const {
                auto new_node = std::make_shared<BinaryOp>(root, other.root, Primitive::Multiply);
                return Expression(new_node);
            }
            Expression operator/(const Expression& other) const {
                auto new_node = std::make_shared<BinaryOp>(root, other.root, Primitive::Divide);
                return Expression(new_node);
            }
            // unary operations
            Expression operator-() const {
                auto new_node = std::make_shared<UnaryOp>(root, Primitive::Negate);
                return Expression(new_node);
            }
    };

    Expression exp(const Expression& other) {
        auto new_node = std::make_shared<UnaryOp>(other.get_root(), Primitive::Exp);
        return Expression(new_node);
    }
    Expression sqrt(const Expression& other) {
        auto new_node = std::make_shared<UnaryOp>(other.get_root(), Primitive::Sqrt);
        return Expression(new_node);
    }
    Expression pow(const Expression& base, const double exp) {
        auto new_node = std::make_shared<UnaryOp>(base.get_root(), Primitive::Pow);
        return Expression(new_node);
    }
}

#endif // EXPRESSION_H
