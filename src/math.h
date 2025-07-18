#ifndef MATH_H
#define MATH_H

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

    // ==== DEFINITIONS ====
    template<typename T>
    struct tensor;

    template<typename T>
    struct view;

    // ==== IMPLEMENTATIONS ====
    template<typename T>
    struct tensor {
        T* data;
        std::vector<std::size_t> shape;
        std::vector<std::size_t> strides;
        std::size_t size;

        // Variadic constructor for shape
        template<typename... Dims>
        tensor(Dims... dims)
            : shape{static_cast<std::size_t>(dims)...}
        {
            size = std::accumulate(shape.begin(), shape.end(), std::size_t(1), std::multiplies<std::size_t>());
            data = new T[size];
            compute_strides();
        }

        // Constructor from shape vector
        tensor(const std::vector<std::size_t>& dims)
            : shape(dims)
        {
            size = std::accumulate(shape.begin(), shape.end(), std::size_t(1), std::multiplies<std::size_t>());
            data = new T[size];
            compute_strides();
        }

        tensor(const tensor& other)
            : data(new T[other.size]), shape(other.shape), strides(other.strides), size(other.size)
        {
            std::copy(other.data, other.data + other.size, data);
        }

        tensor& operator=(const tensor& other) {
            if (this != &other) {
                delete[] data;
                shape = other.shape;
                strides = other.strides;
                size = other.size;
                data = new T[size];
                std::copy(other.data, other.data + other.size, data);
            }
            return *this;
        }

        tensor(tensor&& other) noexcept
            : data(other.data), shape(std::move(other.shape)), strides(std::move(other.strides)), size(other.size)
        {
            other.data = nullptr;
            other.size = 0;
        }

        tensor& operator=(tensor&& other) noexcept {
            if (this != &other) {
                delete[] data;
                data = other.data;
                shape = std::move(other.shape);
                strides = std::move(other.strides);
                size = other.size;
                other.data = nullptr;
                other.size = 0;
            }
            return *this;
        }

        ~tensor() {
            delete[] data;
        }

        // ND indexing operator
        template<typename... Indices>
        T& operator[](Indices... indices) {
            static_assert(sizeof...(Indices) == 0 || sizeof...(Indices) == sizeof...(Indices), "Invalid number of indices");
            std::array<std::size_t, sizeof...(Indices)> idx{static_cast<std::size_t>(indices)...};
            return data[offset(idx)];
        }

        template<typename... Indices>
        const T& operator[](Indices... indices) const {
            static_assert(sizeof...(Indices) == 0 || sizeof...(Indices) == sizeof...(Indices), "Invalid number of indices");
            std::array<std::size_t, sizeof...(Indices)> idx{static_cast<std::size_t>(indices)...};
            return data[offset(idx)];
        }

        view<T> slice(const std::vector<std::size_t>& start, const std::vector<std::size_t>& end) {
            if (start.size() != shape.size() || end.size() != shape.size())
                throw std::invalid_argument("Slice start/end must match tensor rank");
            std::vector<std::size_t> new_shape(shape.size());
            std::size_t new_offset = 0;
            for (std::size_t i = 0; i < shape.size(); ++i) {
                if (end[i] <= start[i] || end[i] > shape[i])
                    throw std::out_of_range("Invalid slice range");
                new_shape[i] = end[i] - start[i];
                new_offset += start[i] * strides[i];
            }
            return view<T>{data, new_shape, strides, new_offset};
        }

    private:
        void compute_strides() {
            strides.resize(shape.size());
            if (shape.empty()) return;
            strides.back() = 1;
            for (int i = static_cast<int>(shape.size()) - 2; i >= 0; --i) {
                strides[i] = strides[i + 1] * shape[i + 1];
            }
        }

        template<std::size_t N>
        std::size_t offset(const std::array<std::size_t, N>& idx) const {
            if (N != shape.size())
                throw std::out_of_range("Incorrect number of indices");
            std::size_t off = 0;
            for (std::size_t i = 0; i < N; ++i) {
                if (idx[i] >= shape[i])
                    throw std::out_of_range("Index out of bounds");
                off += idx[i] * strides[i];
            }
            return off;
        }
    };

    template<typename T>
    struct view {
        T* data;
        std::vector<std::size_t> shape;
        std::vector<std::size_t> strides;
        std::size_t offset;

        template<typename... Indices>
        T& operator()(Indices... indices) {
            std::array<std::size_t, sizeof...(Indices)> idx{static_cast<std::size_t>(indices)...};
            std::size_t off = offset;
            for (std::size_t i =0; i < idx.size(); i++)
                off += idx[i] * strides[i];
            return data[off];
        }
    };

}


#endif // MATH_H