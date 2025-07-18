# tall
Build modular and blazingly fast scientific models and pipelines.

## Usage

```c++
#include "tall.h"

int main(int argc, char** argv) {

    std::size_t capacity = 100;     // number of unique components
    tall::registry r(capacity);     // initialize the registry

    // initialize primitives
    auto x = tall::var(r);
    auto y = tall::var(r);
    auto z = tall::var(r);
    auto s = tall::param(r);
    auto r = tall::param(r);
    auto b = tall::param(r);

    // assemble components from primitives
    auto vars  = tall::assemble(r, {x, y, z});
    auto pars = tall::assemble(r, {s, r, b});

    auto particle = tall::assemble(r, {vars, parameters})  // ...and keep building
    
    auto earth = tall::assemble(r, {particle}, 1000);           // ...and building
    auto mars = tall::assemble(r, {particle}, 100);             // ...and building
    auto solarsystem = tall::assemble(r, {earth, mars});        // ...and building

    // allocate the data
    tall::model m;
    if (!allocate(r, m)) {
        tall::log::DEBUG("Allocation failed!");
    }

    auto parts = m[mars, particle]     // access component data using fast random access

    // cach-friendly iterations
    for (auto p : parts) {
        p[x] = p[x] * (p[y] - p[x]);
        p[y] = p[x] * (p[r] - p[z]) - p[y];
        p[z] = p[x] * p[y] - p[b] * p[z];
    }


    return 0;
}
```