#include "test.h"
#include "../src/primitive.h"
#include "../src/entity.h"
#include "../src/expression.h"

#include <thread>
#include <chrono>

using namespace tall;

struct ExpressionTest : public test::Module {

        void build(test::Runner& runner) override {

            runner.unit("Create Expression", []() {
                auto A = Expression(Entity());
                auto B = Expression(Entity());

                auto C = pow(A + B, 2) * A;


                return test::Result::Ok;
            });
        }
};


int main() {

    test::Runner runner;

    runner.push(ExpressionTest());

    runner.run();
}
