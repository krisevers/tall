#include <iostream>
#include <vector>
#include <memory>
#include <typeindex>
#include <string>

#include "../src/tall.h"


// component:   holds information on the allocation and 
// registry:    handles the construction of the model.
// model:       handles the access to data in storage.
// storage:     handles only the allocation and storage of the data.

void* allocate(std::size_t dtype_size, std::size_t array_size) {
    return malloc(dtype_size * array_size);
}

bool deallocate(void* ptr) {
    if (ptr) {
        free(ptr);
        ptr = nullptr;
        return true;
    }
    return false;
}



struct any {
    std::size_t dtype_size;
    std::size_t array_size;

    virtual ~any() = default;
};

template<typename T>
struct component : any {

    component(std::size_t size) {
        dtype_size = sizeof(T);
        array_size = size;
    }

    T& get(void* ptr, std::size_t idx) {
        return static_cast<T*>(ptr)[idx];
    }
};

struct storage {
    std::vector<std::unique_ptr<void, bool(*)(void*)>> ptrs;
    std::vector<any> items;

    void push_back(any comp) {
        void* raw_ptr = allocate(comp.dtype_size, comp.array_size);
        ptrs.push_back(std::unique_ptr<void, bool(*)(void*)>(raw_ptr, deallocate));
        items.push_back(comp);
    }
};


struct point {
    double x;
    double y;
};



int main() {

    auto y = component<point>(100);

    storage s;

    s.push_back(y);



    // compute allocation memory requirements
    // iterate through data components and determine the requirements


}
