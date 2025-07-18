#include "../src/tall.h"

#include <iostream>

int main(int argc, char** argv) {

    std::size_t capacity = 3;
    tall::registry r(capacity);

    tall::ID x = tall::var(r);
    tall::ID y = tall::var(r);
    tall::ID z = tall::var(r);

    tall::log::DEBUG(std::to_string(x.value));
    tall::log::DEBUG(std::to_string(y.value));
    tall::log::DEBUG(std::to_string(z.value));

    return 0;
}