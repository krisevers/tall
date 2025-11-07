#include <iostream>
#include <numeric>
#include <vector>
#include <array>
#include <concepts>

template<typename T>
void print(T value) {
    std::cout << value << "\n";
}

template<typename T>
void print_vector(std::vector<T> value) {
    for (const T& i : value) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

template<typename T>
void print_array(T* value, std::size_t size) {
    for (int i=0; i<size; i++) {
        std::cout << value[i] << " ";
    }
    std::cout << "\n";
}


template<typename T>
struct Tensor {
    T* data;
    std::vector<std::size_t> shape;
    std::vector<std::size_t> strides;
    std::size_t size;

    // Constructor
    template<typename...Dims>
    Tensor(Dims...dims)
        : shape{static_cast<std::size_t>(dims)...}
    {
        size = std::accumulate(shape.begin(), shape.end(), std::size_t(1), std::multiplies<std::size_t>());
        data = new T[size];
        compute_strides();
    }

    Tensor(const std::vector<std::size_t>& dims)
        : shape(dims)
    {
        size = std::accumulate(shape.begin(), shape.end(), std::size_t(1), std::multiplies<std::size_t>());
        data = new T[size];
        compute_strides();
    }

    // Copy constructor
    Tensor(const Tensor& other)
        : data(new T[other.size]), shape(other.shape), strides(other.strides), size(other.size)
    {
        std::copy(other.data, other.data + other.size, data);
    }

    Tensor& operator=(const Tensor& other) {
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

    // Move constructor
    Tensor(Tensor&& other) noexcept
    : data(other.data), shape(std::move(other.shape)), strides(std::move(other.strides)), size(other.size)
    {
        other.data = nullptr;
        other.size = 0;
    }

    Tensor& operator=(Tensor&& other) noexcept {
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



    // Destructor
    ~Tensor() {
        delete[] data;
    }


    // void* allocate(Allocator allocator) {

    // }


   private:
        void compute_strides() {
            strides.resize(shape.size());
            if (shape.empty()) return;
            strides.back() = 1;
            for (int i=static_cast<int>(shape.size()) - 2; i >= 0; --i) {
                strides[i] = strides[i + 1] * shape[i + 1];
            }
        }

        template<std::size_t N>
        std::size_t offset(const std::array<std::size_t, N>& idx) const {
            if (N != shape.size()) {
                throw std::out_of_range("Incorrect number of indices");
            }
            std::size_t off = 0;
            for (std::size_t i=0; i<N; ++i) {
                std::cout << i << "\n";
                if (idx[i] >= shape[i]) {
                    throw std::out_of_range("Index out of bounds");
                }
                off += idx[i] * strides[i];
            }
            return off;
        }

};


#include "allocator.h"

int main() {

    Tensor x = Tensor<double>(10, 5, 2);
    Tensor y = Tensor<double>(5, 5);

    std::size_t gB = (std::size_t)1024 * 1024 * 1024;
    Allocator alloc(gB);

    auto ptr = alloc.allocate_page(0);

    print(ptr);

    return 0;
}
