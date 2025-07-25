#include "../src/tall.h"

#include <iostream>

int main(int argc, char** argv) {

    std::size_t capacity = 100;
    tall::registry r(capacity);

    auto x = tall::real(r);
    auto y = tall::real(r);
    auto z = tall::real(r);

    auto particle = tall::assemble(r, {x, y, z});
    
    auto earth = tall::assemble(r, {particle}, 1000);
    auto mars = tall::assemble(r, {particle}, 100);

    auto solarsystem = tall::assemble(r, {earth, mars});




    // we want paged storage for large numbers of entities.
    // m[particle, x] << this kind of syntax should return all x's which belong to particle. 
    // r.allocate()



    tall::print(r);

    return 0;
}