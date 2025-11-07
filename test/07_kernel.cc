#include <vector>
#include <initializer_list>
#include <queue>

#include "../src/types.h"
#include "../src/registry.h"


namespace tall {

    enum EXP {
        ADD,
        SUB,
        MUL,
        DIV,
    };

    struct expression {
        std::size_t lhs, rhs;
        EXP etype;
        std::unique_ptr<std::queue<expression>> ops;

        // Constructor for binary operations (ID + ID)
        expression(ID lhs, ID rhs, EXP etype) : lhs(lhs), rhs(rhs), etype(etype) {
            ops = std::make_unique<std::queue<expression>>();
        }

        // Constructor for unary operations or compound expressions
        expression(ID lhs, EXP etype) : lhs(lhs), rhs(0), etype(etype) {
            ops = std::make_unique<std::queue<expression>>();
        }

        // Copy constructor
        expression(const expression& other) : lhs(other.lhs), rhs(other.rhs), etype(other.etype) {
            if (other.ops) {
                ops = std::make_unique<std::queue<expression>>(*other.ops);
            }
        }

        // Move constructor
        expression(expression&& other) noexcept : lhs(other.lhs), rhs(other.rhs), etype(other.etype), ops(std::move(other.ops)) {}

        // Assignment operators
        expression& operator=(const expression& other) {
            if (this != &other) {
                lhs = other.lhs;
                rhs = other.rhs;
                etype = other.etype;
                if (other.ops) {
                    ops = std::make_unique<std::queue<expression>>(*other.ops);
                }
            }
            return *this;
        }

        expression& operator=(expression&& other) noexcept {
            if (this != &other) {
                lhs = other.lhs;
                rhs = other.rhs;
                etype = other.etype;
                ops = std::move(other.ops);
            }
            return *this;
        }
    };

    // Operator overloads for ID + ID
    expression operator+(ID lhs, ID rhs) {
        return expression(lhs, rhs, ADD);
    }

    expression operator-(ID lhs, ID rhs) {
        return expression(lhs, rhs, SUB);
    }

    expression operator*(ID lhs, ID rhs) {
        return expression(lhs, rhs, MUL);
    }

    expression operator/(ID lhs, ID rhs) {
        return expression(lhs, rhs, DIV);
    }

    // Operator overloads for chaining: expression + ID
    expression operator+(expression lhs, ID rhs) {
        expression new_exp(lhs.lhs, rhs, ADD);
        // Copy existing operations
        if (lhs.ops && !lhs.ops->empty()) {
            while (!lhs.ops->empty()) {
                new_exp.ops->push(std::move(lhs.ops->front()));
                lhs.ops->pop();
            }
        }
        return new_exp;
    }

    expression operator-(expression lhs, ID rhs) {
        expression new_exp(lhs.lhs, rhs, SUB);
        if (lhs.ops && !lhs.ops->empty()) {
            while (!lhs.ops->empty()) {
                new_exp.ops->push(std::move(lhs.ops->front()));
                lhs.ops->pop();
            }
        }
        return new_exp;
    }

    struct kernel {
        std::unique_ptr<std::queue<expression>> ops;


        kernel(std::initializer_list<expression>) {
            // save all expressions
        }
    };

}


int main() {



    tall::registry r(10);

    auto x = tall::var(r);
    auto y = tall::var(r);
    auto z = tall::var(r);

    tall::expression e = (x + y) - z;

    tall::kernel k{
        {
            e = (x + y) - z,
            e = (z * y) - x,
        }
    };

    tall::log::DEBUG(std::to_string(e.lhs));
    tall::log::DEBUG(std::to_string(e.rhs));




    // insight: call the functions 'primitives'
    // insight: call the data types 'primitives'

    // insight: after initialization there is no need to get back the data as a result.
    // data-manipulation can be performed by calling specific optimized functions on the data (e.g. view)


    // compute allocation memory requirements
    // iterate through data components and determine the requirements


}
