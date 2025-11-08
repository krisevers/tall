// #include <assert>
#include <iostream>
#include <chrono>
#include <functional>
#include <format>
#include <string>

#include "../src/logger.h"

namespace test {

    // forward declarations
    enum class Result;
    using Fn = std::function<Result()>;
    class Module;
    class Runner;

    // implementations
    enum class Result {
        Ok,
        Err,
    };

    class Runner {
        std::vector<std::unique_ptr<Module>> modules;
        std::vector<std::pair<std::string, Fn>> functions;

        public:

            template<typename M>
            Runner& push(M&& module) {
                auto m = std::make_unique<M>(std::forward<M>(module));
                m->build(*this);
                modules.push_back(std::move(m));
                return *this;
            }

            Runner& unit(std::string name, Fn function) {
                functions.push_back({name, function});
                return *this;
            }

            void run() {
                tall::log::INFO(std::format("=================="));
                tall::log::INFO(std::format("Running {} tests...", functions.size()));
                auto start = std::chrono::steady_clock::now();

                for (std::size_t i = 0; i < functions.size(); i++) {
                    if (functions[i].second() == Result::Ok) {
                        tall::log::INFO(std::format("{}", functions[i].first));
                    } else {
                        tall::log::ERROR(std::format("{}", functions[i].first));
                    };
                }

                auto end = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start
                );

                tall::log::INFO(std::format("All tests completed in {}ms", elapsed.count()));
            }

    };

    struct Module {
        virtual ~Module() = default;
        virtual void build(class Runner& runner) = 0;
    };
}
