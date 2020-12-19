#include "my_vector.h"
#include <cassert>
#include <iostream>

int
main()
{
    // Zero test
    My_vector<int> x;
    assert(x.size() == 0);
    assert(x.capacity() == 1);
    assert(x.empty() == true);
    
    // Reserve test
    x.reserve(1000);
    assert(x.capacity() == 1000);
    
    // Resize test
    x.resize(50);
    assert(x.size() == 50);
    assert(x.empty() == false);
    assert(x[40] == 0);
    
    // Clear test
    x.clear();
    assert(x.size() == 0);
    assert(x.empty() == true);
    assert(x.capacity() == 1000);
    
    // Push_back & [] test
    int a = 5;
    x.push_back(1);
    x.push_back(a);
    x.push_back(std::move(a));
    assert(x.size() == 3);
    assert(x[0] == 1);
    assert(x[1] == 5);
    assert(x[2] == 5);
    
    // Pop_back test
    x.pop_back();
    assert(x.size() == 2);
    
    // Emplace_back test
    My_vector<std::pair<std::string, int>> y;
    y.emplace_back("qwerty", 123);
    
    
    // *** Iterators ***
    x.clear();
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);
    int i;
    // Iterator test
    i = 1;
    for (auto it = x.begin(); it != x.end(); ++it, i++) {
        assert(*it == i);
    }
    // Reverse iterator test
    i = 5;
    for (auto it = x.rbegin(); it != x.rend(); ++it, i--) {
        assert(*it == i);
    }
    
    std::cout << "All tests passed" << std::endl;
    return 0;
}