#include "test.h"
#include "../src/primitive.h"

#include <thread>
#include <chrono>


struct PrimitiveTest : public test::Module {

    void build(test::Runner& runner) override {

        runner.unit("a_test", []() {
            return test::Result::Err;
        });

        runner.unit("another_test", []() {
            return test::Result::Ok;
        });
    }
};


int main() {

    test::Runner runner("tall");

    runner.push(PrimitiveTest());

    runner.run();
}
