#include "../src/math.h"

#include <iostream>

int main(int argc, char** argv) {

    tall::tensor<double> t(4, 5, 6);

    tall::view s = t.slice({1, 2, 0}, {3, 5, 6});

    return 0;
}