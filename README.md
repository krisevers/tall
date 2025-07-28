# tall
Build modular and blazingly fast scientific models and pipelines.

## Usage

```c++
#include "tall.h"

int main(int argc, char** argv) {

    std::size_t capacity = 100;     // number of unique components
    tall::registry r(capacity);     // initialize the registry

    // initialize primitives
    auto x = r.var();
    auto y = r.var();
    auto z = r.var();
    auto sigma = r.var();
    auto beta  = r.var();
    auto rho   = r.var();

    auto dx = sigma*(y-x);
    auto dy = x*(rho-z)-y;
    auto dz = x*y - beta*y;
    tall::kernel lorenz({dx, dy, dz});

    // assemble components from primitives
    auto vars  = r.assemble({x, y, z});
    auto pars = r.assemble({s, r, b});

    auto particle = r.assemble({vars, parameters})  // ...and keep building

    auto earth = r.assemble({particle}, 1000);           // ...and building
    auto mars = r.assemble({particle}, 100);             // ...and building
    auto solarsystem = r.assemble({earth, mars});        // ...and building

    // allocate the data
    tall::model m;
    if (!allocate(r, m)) {
        tall::log::DEBUG("Allocation failed!");
    }

    auto mars_parts = m[mars, particle]     // access component data using fast random access

    // cach-friendly iterations
    auto dx = sigma*(y-x);
    auto dy = x*(rho-z)-y;
    auto dz = x*y - beta*y;
    tall::kernel lorenz({dx, dy, dz});

    return 0;
}
```
