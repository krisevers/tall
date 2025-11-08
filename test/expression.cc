#include "test.h"
#include "../src/primitive.h"
#include "../src/entity.h"

#include <thread>
#include <chrono>


struct EntityTest : public test::Module {

    void build(test::Runner& runner) override {
        using namespace tall;

        runner.unit("create_entity", []() {

            auto x = Entity();
            auto y = Entity();

            if (x == 0 && y == 1) {
                return test::Result::Ok;
            }
            return test::Result::Err;
        });
    }
};

struct PrimitiveTest : public test::Module {

    void build(test::Runner& runner) override {

        runner.unit("create_primitive", []() {

            using namespace tall;

            Variable x = Variable(Entity());
            Variable y = Variable(Entity());

            if (x.id == 0 && y.id == 1) {
                return test::Result::Ok;
            }
            return test::Result::Err;
        });
    }
};

// struct ExpressionTest : public test::Module {

//     void build(test::Runner& runner) override {

//         runner.unit("")
//     }
// }


int main() {

    test::Runner runner;

    runner.push(EntityTest());
    runner.push(PrimitiveTest());

    runner.run();
}
