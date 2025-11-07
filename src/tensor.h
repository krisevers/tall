#ifndef TENSOR_H
#define TENSOR_H

#include <vector>
#include <initializer_list>
#include <numeric>
#include <stdexcept>
#include <cstddef>
#include <utility>
#include <algorithm>
#include <array>

#include "logger.h"

namespace tall {

    template<typename T>
    struct tensor {
        T* data;
        std::vector<std::size_t> shape;
        std::vector<std::size_t> strides;
        std::size_t size;

        // Variadic constructor for shape
        tensor(Dims... dims)
            : shape{static_cast<std::size_t>(dims)...}
        {
            size = std::accumulate(shape.begin(), shape.end(), std::size_t(1), std::multiplies<std::size_t>());
        }

    template<typename T>
    struct view;



}

#endif // TENSOR_H
