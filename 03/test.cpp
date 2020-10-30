#include "array.h"
#include <iostream>
#include <sstream>
#include <cstring>


int default_init_test() {
    try {
        Array x;
    }
    catch(const std::exception &err) {
        return 1;
    }
    return 0;
}

int init_test() {
    try {
        Array(std::vector<unsigned>(5, 4));
    }
    catch (const std::exception &err) {
        return 1;
    }
    return 0;
}

int view_test() {
    Array x = Array(std::vector<unsigned>(5, 4));
    Array y;
    try {
        y = x;
    } 
    catch(const std::exception &err) {
        return 1;    
    }
    
    try {
        x[0][0][0][0][0] = 1;
        y[0][0][0][0][0] = 0;    
        if (x[0][0][0][0][0] != y[0][0][0][0][0]) 
            throw std::logic_error("");
        // std::cout << "ok\n";

    }
    catch (const std::logic_error &err) {
        return 1;
    }/*
    catch (std::exception err) {
        std::cout << "ok\n";
        fflush(stdout);
        return 1;
    }*/
    return 0;
}

int copy_test() {
    Array x = Array(std::vector<unsigned>(5, 4));
    Array y;
    try {
        y = x.deep_copy();
    } 
    catch(const std::exception &err) {
        return 1;    
    }
    try {
        x[0][0][0][0][0] = 1;
        y[0][0][0][0][0] = 0;
        if (x[0][0][0][0][0] == y[0][0][0][0][0]) 
            throw std::logic_error("");
    }
    catch (const std::logic_error &err) {
        return 1;
    }/*
    catch (std::exception err) {
        return 1;
    }*/
    return 0;
}

int T_test() {
    Array x(std::vector<unsigned>(2, 3));
    Array y(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            y[j][i] = x[i][j] = i*3 + j;
        }
    }
    if (x.T() == y) return 0;
    return 1;
}

int int_assign_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[0][0] = 5;
    } 
    catch(const std::exception &err) {
        return 1;
    }
    return 0;
}

int invalid_int_assign_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[0] = 5;
    } 
    catch(const std::logic_error &err) {
        if (std::string("Assignment an integer to an array") == err.what()) return 0;
    }
    return 1;
}

int array_assign_test() {
    Array x(std::vector<unsigned>(2, 3));
    Array y(std::vector<unsigned>(1, 3));
    y[0] = 0;
    y[1] = 1;
    y[2] = 2;
    try {
        x[0] = y;
    } 
    catch(const std::exception &err) {
        return 1;
    }
    return 0;
}

int invalid_array_assign_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[0] = x;
    } 
    catch(const std::logic_error &err) {
        if (std::string("Assigning arrays of different shape") == err.what()) return 0;
    }
    return 1;
}

int view_compare_test() {
    Array x(std::vector<unsigned>(2, 3));
    Array y(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
            y[i][j] = i*3 + j;
            
        }
    }
    //std::cout << "ok\n";
    // fflush(stdout);

    if (x == y) return 0;
    return 1;
}

int copy_compare_test() {
    Array x(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
        }
    }
    Array y = x;
    if (x == y) return 0;
    return 1;
}

int print_test(bool silent) {
    Array x(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
        }
    }
    std::stringstream out;
    out << x;
    std::string str = out.str();
    return 0;
    if (str == "[[    0     1     2]\n[    3     4     5]\n[    6     7     8]]") return 0;
    return 1;
}

int index_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[1][2];
    } 
    catch(const std::exception &err) {
        return 1;
    }
    return 0;
}

int negative_index_test(){
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[-1][2];
    } 
    catch(const std::exception &err) {
        return 1;
    }
    return 0;
}

int index_out_of_range_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[0][5];
    } 
    catch(const std::out_of_range &err) {
        return 0;
    }
    return 1;
}

int too_many_indexes_test() {
    Array x(std::vector<unsigned>(2, 3));
    try {
        x[0][0][0];
    } 
    catch(const std::logic_error &err) {
        if(std::string("Array doesn't have that many dimensions") == err.what()) return 0;
    }
    return 1;
}

int add_tests() {
    Array x(std::vector<unsigned>(2, 3));
    Array y(std::vector<unsigned>(2, 3));
    Array res1(std::vector<unsigned>(2, 3));
    Array res2(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
            y[i][j] = i*3 + j + 9;
            res1[i][j] = 9 + 6*i + 2*j;
            res2[i][j] = 9 + 3*i + j;
        }
    }
    try {
        if (x + y != res1) return 1;
        if (y + x != res1) return 1;
        if (x + 9 != res2) return 1;
        if (9 + x != res2) return 1;
        return 0;
    }
    catch(const std::exception &err) {
        return 1;
    }
}

int sub_tests() {
    Array x(std::vector<unsigned>(2, 3));
    Array y(std::vector<unsigned>(2, 3));
    Array res1(std::vector<unsigned>(2, 3));
    Array res2(std::vector<unsigned>(2, 3));
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
            y[i][j] = 9 - i*3 - j;
            res1[i][j] = 9 - 6*i - 2*j;
            res2[i][j] = 9 - 3*i - j;
        }
    }
    try {
        if (x - y != -1 * res1) return 1;
        if (y - x != res1) return 1;
        if (x - 9 != -1 * res2) return 1;
        if (9 - x != res2) return 1;
        return 0;
    }
    catch(const std::exception &err) {
        return 1;
    }
}

int mult_tests() {
    Array x(std::vector<unsigned>(2, 3));
    Array res(std::vector<unsigned>(2, 3));
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            x[i][j] = i*3 + j;
            res[i][j] = 9*i + 3*j;
        }
    }
    try {
        if (x * 3 != res) return 1;
        if (3 * x != res) return 1;
        return 0;
    }
    catch(const std::exception &err) {
        return 1;
    }
}

int math_tests(bool silent) {
    int res = 0;
    if (sub_tests()) {
        res++;
        if (!silent) std::cout << "Substraction tests failed" << std::endl; 
    }
    if (add_tests()) {
        res++;
        if (!silent) std::cout << "Addition tests failed" << std::endl; 
    }
    if (mult_tests()) {
        res++;
        if (!silent) std::cout << "Multiply tests failed" << std::endl; 
    }
    if (copy_compare_test()) {
        res++;
        if (!silent) std::cout << "Copy compare test failed" << std::endl; 
    }
    if (view_compare_test()) {
        res++;
        if (!silent) std::cout << "View compare test failed" << std::endl; 
    }
    /*if (T_test()) {
        res++;
        if (!silent) std::cout << "Transposition test failed" << std::endl; 
    }*/
    return res;
};

int init_tests(bool silent) {
    int res = 0;
    if (default_init_test()) {
        res++;
        if (!silent) std::cout << "Default init test failed" << std::endl; 
    }
    if (init_test()) {
        res++;
        if (!silent) std::cout << "Init test failed" << std::endl; 
    }
    if (view_test()) {
        res++;
        if (!silent) std::cout << "View test failed" << std::endl; 
    }
    if (copy_test()) {
        res++;
        if (!silent) std::cout << "Copy test failed" << std::endl; 
    }
    return res;
}

int index_tests(bool silent) {
    int res = 0;
    if (index_test()) {
        res++;
        if (!silent) std::cout << "Index test failed" << std::endl; 
    }
    if (negative_index_test()) {
        res++;
        if (!silent) std::cout << "Negative index test failed" << std::endl; 
    }
    if (index_out_of_range_test()) {
        res++;
        if (!silent) std::cout << "Index out of test failed" << std::endl; 
    }
    if (too_many_indexes_test()) {
        res++;
        if (!silent) std::cout << "Too many indexes test failed" << std::endl; 
    }
    return res;
}

int assign_tests(bool silent) {
    int res = 0;
    if (int_assign_test()) {
        res++;
        if (!silent) std::cout << "Int assign test failed" << std::endl; 
    }
    if (array_assign_test()) {
        res++;
        if (!silent) std::cout << "Array assign test failed" << std::endl; 
    }
    if (invalid_int_assign_test()) {
        res++;
        if (!silent) std::cout << "Invalid int assign test failed" << std::endl; 
    }
    if (invalid_array_assign_test()) {
        res++;
        if (!silent) std::cout << "Invalid array assign test failed" << std::endl; 
    }
    return res;
}

int col_test(bool silent) {
    Array x(std::vector<unsigned>(2, 3));
    if (x.cols() != 3) return 1;
    return 0;
}

int row_test(bool silent) {
    Array x(std::vector<unsigned>(2, 3));
    if (x.rows() != 3) return 1;
    return 0;
}

int main(int argc, char **argv) {
    bool silent = false;
    if (argc > 1) {
        if (!strcmp(argv[1], "silent")) silent = true; 
    }
    int res = 0;
    res += init_tests(silent);
    res += assign_tests(silent);
    res += math_tests(silent);
    res += index_tests(silent);
    res += print_test(silent);
    res += row_test(silent);
    res += col_test(silent);
    
    if (res) {
        std::cout << res << " tests failed" << std::endl;
    } else {
        std::cout << "All tests passed" << std::endl;
    }
    return 0;
}