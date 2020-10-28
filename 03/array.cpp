#include "array.h"
#include <iostream>
#include <exception>
#include <stdexcept>
#include <cstddef>



std::vector<unsigned> Array::get_shape() {
    return this->shape;
}

unsigned Array::get_dim() {
    return this->dim;
}

Array& Array::operator*=(const int x) {
    try
    {
        iter_init();
        while(1) {
            next() *= x;
        }
    }
    catch(std::out_of_range err) 
    {
        return *this;
    }
}

Array& Array::operator+=(const Array& x) {
    if (dim != x.dim) {
        throw std::logic_error("Addition of arrays of different shape");
    }
    for (unsigned i = 0; i < dim; i++) {
        if (shape[i] != x.shape[i]) {
            throw std::logic_error("Addition of arrays of different shape");
        }
    }
    try
    {
        iter_init();
        x.iter_init();
        while(1) {
            next() += x.next();
        }
    }
    catch(std::out_of_range err) 
    {
        return *this;
    }
}

Array& Array::operator-=(const Array& x) {
    if (dim != x.dim) {
        throw std::logic_error("Substruction of arrays of different shape");
    }
    for (unsigned i = 0; i < dim; i++) {
        if (shape[i] != x.shape[i]) {
            throw std::logic_error("Substruction of arrays of different shape");
        }
    }
    try
    {
        iter_init();
        x.iter_init();
        while(1) {
            next() -= x.next();
        }
    }
    catch(std::out_of_range err) 
    {
        return *this;
    }
}

Array& Array::operator+=(const int x) {
    try
    {
        iter_init();
        while(1) {
            next() += x;
        }
    }
    catch(std::out_of_range err) 
    {
        return *this;
    }
}

Array& Array::operator-=(const int x) {
    try
    {
        iter_init();
        while(1) {
            next() -= x;
        }
    }
    catch(std::out_of_range err) 
    {
        return *this;
    }
}

Array& Array::operator+(const int x) const{
    Array *res = &deep_copy();
    return *res += x;
}

Array& Array::operator-(const int x) const{
    Array *res = &deep_copy();
    return *res -= x;
}

Array& Array::operator*(const int x) const{
    Array *res = &deep_copy();
    return *res *= x;
}

Array& operator+(const int x, const Array& y) {
    return y+x;
}

Array& operator-(const int x, const Array& y) {
    return (y-x) *= -1;
}

Array& operator*(const int x, const Array& y) {
    return y*x;
}

Array& Array::operator+(const Array& x) const{
    Array *res = &deep_copy();
    return *res += x;
}

Array& Array::operator-(const Array& x) const{
    Array *res = &deep_copy();
    return *res -= x;
}

bool Array::operator==(const Array& x) const{
    if (dim != x.dim) {
        return false;
    }
    if (!dim) {
        return data.get()[offset] == x.data.get()[x.offset];
    }
    for (unsigned i = 0; i < dim; i++) {
        if (x.shape[i] != shape[i]) {
            return false;
        }
    }
    try
    {
        iter_init();
        x.iter_init();
        while(1) {
            if (next() != x.next()) {
                return false;
            }
        }
    }
    catch(std::out_of_range err) 
    {
        return true;
    }
}

bool Array::operator!=(const Array& x) const{
    return !(*this == x);
}

Array& Array::operator[](int index) const{
    if (!dim) {
        throw std::logic_error("Array doesn't have that many dimensions");
    }
    if(index >= (int)shape[0] || index < -(int)shape[0]) {
        throw std::out_of_range("");
    }
    int true_index = (index < 0) ? (shape[0] + index) : index;
    Array *res = new Array;
    res->data = data;
    res->shape = std::vector<unsigned int>(++(shape.begin()), shape.end());
    res->offset = offset + true_index * strides[0];
    res->strides = std::vector<int>(++(strides.begin()), strides.end());
    res->dim = dim - 1;
    res->full_data = 0;
    return *res;
}

Array::operator int() {
    if (dim == 0) {
        return data.get()[offset];
    } else {
        throw std::logic_error("Usage of array as int");
    }
}

std::ostream &operator<<(std::ostream& out, const Array& x) {
    out.width(1);
    if(x.dim == 0) {
        out << x.data.get()[0] << std::endl;
        return out;
    }
    out << "[";
    for (unsigned i = 0; i < x.shape[0]; i++) {
        if (x.dim != 1) {
            if (i + 1 != x.shape[0]) out << x[i] << std::endl;
            else out << x[i];
        } else {
            out.width(5);
            if (i + 1 != x.shape[0]) out << (int)x[i] << ' ';
            else out << (int)x[i];
        }
    }
    out.width(1);
    out << "]";
    return out;
}

Array& Array::deep_copy() const{
    Array *res = new Array(shape);
    res->iter_init();
    iter_init();
    try 
    {
        while(1) {
            // std::cout << res->iter[0] << ' ' << res->shape[0]<<"ok" << std::endl;
            // fflush(stdout);
    
            res->next() = next();
        }
    }
    catch (std::out_of_range err) {
        // std::cout << "ok" << std::endl;
        // fflush(stdout);
    
        return *res;
    }
} 

Array::Array(std::vector<unsigned> shape) {
    this->shape = shape;
    dim = shape.size();
    this->strides = std::vector<int>(dim);
    
    offset = 0;
    full_data = true;
    long long n = 1;
    for (int i = dim - 1; i >= 0; i--) {
        strides[i] = n;
        n *= shape[i];
    }
    data = std::shared_ptr<int>(new int [n], std::default_delete<int[]>());
}

Array& Array::T() {
    if (dim != 2) {
        throw std::logic_error("Only matrix(2-dimensional arrays) transpose is supported");
    }
    Array *res = &(this->deep_copy());
    int tmp = res->strides[0];
    res->strides[0] = res->strides[1];
    res->strides[1] = tmp;
    return *res;
}

void Array::iter_init() const {
    iter = std::vector<unsigned>(dim, 0);
}

int& Array::next() {
    if (iter[0] == shape[0]) {
        throw std::out_of_range("");
    }
    int index = offset;
    for (unsigned i = 0; i < dim; i++) {
        index += strides[i] * iter[i];
    }
    for (int i = dim - 1; i >= 0; i--) {
        iter[i]++;
        if (iter[i] != shape[i] || !i) break;
        iter[i] = 0;
    }
    return data.get()[index];
}

int& Array::next() const {
    if (iter[0] == shape[0]) {
        throw std::out_of_range("");
    }
    int index = offset;
    for (unsigned i = 0; i < dim; i++) {
        index += strides[i] * iter[i];
    }
    for (int i = dim - 1; i >= 0; i--) {
        iter[i]++;
        if (iter[i] != shape[i] || !i) break;
        iter[i] = 0;
    }
    return data.get()[index];
}

Array::Array() : dim(0), offset(0), full_data(true) {
    data = std::shared_ptr<int>(new int[1], std::default_delete<int[]>());
    shape = std::vector<unsigned>();
    strides = std::vector<int>();
}

Array& Array::operator=(const int x) {
    if (dim) {
        throw std::logic_error("Assignment an integer to an array");
    }
    data.get()[offset] = x;
    return *this;
}

Array& Array::operator=(const Array& x) {
    if (full_data) {
        data = x.data;
        shape = x.shape;
        dim = x.dim;
        full_data = x.full_data;
        offset = x.offset;
        strides = x.strides;
        return *this;
    } else {
        if (dim != x.dim) {
            throw std::logic_error("Assigning arrays of different shape");
        }
        for (unsigned i = 0; i < dim; i++) {
            if (shape[i] != x.shape[i]) {
                throw std::logic_error("Assigning arrays of different shape");
            }
        }
        try
        {
            iter_init();
            x.iter_init();
            while(1) {
                next() = x.next();
            }
        }
        catch(std::out_of_range err) 
        {
            return *this;
        }
    }
}

unsigned Array::rows() const{
    if (!dim) {
        throw std::logic_error("Array is zero-dimensional");
    }
    return shape[0];
}


unsigned Array::cols() const{
    if (dim < 2) {
        throw std::logic_error("Array is zero or one -dimensional");
    }
    return shape[1];
}


unsigned Array::slices() const{
    if (dim < 3) {
        throw std::logic_error("Array doesn't have 3 dimensions");
    }
    return shape[2];
}
