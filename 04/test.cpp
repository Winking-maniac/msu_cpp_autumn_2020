#include "bigint.h"
#include <iostream>
#include <sstream>

int
construct_tests()
{
    try
    {
        BigInt a;
        BigInt b = 1;
        BigInt c = -1;
        BigInt d = BigInt("-1120103295958198");
        BigInt e = BigInt("+1120103295958198");
        BigInt f = BigInt("00001120103295958198");
        BigInt g = BigInt("1120103295958198");
        BigInt h = BigInt(g);
        g = 0;
        if (h == 0) {
            std::cout << "Construct tests failed" << std::endl;
            return 1;
        }
        h = std::move(g);
    }
    catch(std::exception)
    {
        std::cout << "Construct tests failed" << std::endl;
        return 1;
    }
    std::cout << "Construct tests passed" << std::endl;
    return 0;
}

int
math_tests()
{
    int res = 0;
    bool fail = false;
    BigInt a("12345678901234567890");
    BigInt b("-12345678900987654321");
    BigInt c("123456789012345678901234567890");
    
    
    if (c+a != BigInt("123456789024691357802469135780")) res++; 
    if (a+b != BigInt(246913569)) res++; 
    if (b+c != BigInt("123456789000000000000246913569")) res++; 
    if (100+c != BigInt("123456789012345678901234567990")) res++; 
    if (a+200 != BigInt("12345678901234568090")) res++; 
    if (b+200 != BigInt("-12345678900987654121")) res++; 
    
    if (res) {
        std::cout << "Addition tests failed" << std:: endl;
        fail = true;
    }
    res = 0;
    
    if (a-c != BigInt("-123456789000000000000000000000")) res++; 
    if (a-b != BigInt("24691357802222222211")) res++; 
    if (b-c != BigInt("-123456789024691357802222222211")) res++; 
    if (100-c != BigInt("-123456789012345678901234567790")) res++; 
    if (a-200 != BigInt("12345678901234567690")) res++; 
    if (b-200 != BigInt("-12345678900987654521")) res++; 
    if (100-b != BigInt("12345678900987654421")) res++; 
    
    if (res) {
        std::cout << "Substruction tests failed" << std:: endl;
        fail = true;
    }
    res = 0;
    
    if (a*c != BigInt("1524157875323883675034293577501905199875019052100")) res++; 
    if (a*b != BigInt("-152415787529340051862673374211126352690")) res++; 
    if (b*c != BigInt("-1524157875293400518641975320862673374211126352690")) res++; 
    if (b*b != BigInt("152415787526291736223502514857789971041")) res++; 
    if (100*c != BigInt("12345678901234567890123456789000")) res++; 
    if (a*200 != BigInt("2469135780246913578000")) res++; 
    if (b*200 != BigInt("-2469135780197530864200")) res++; 
    if (100*b != BigInt("-1234567890098765432100")) res++; 
    
    if (res) {
        std::cout << "Multiplication tests failed" << std:: endl;
        fail = true;
    }
    
    if (fail) {
       std::cout << "Math tests failed" << std:: endl;
       return 1;
    }
    std::cout << "Math tests passed" << std:: endl;
    return 0;
}

int
comp_tests()
{
    BigInt a = 1;
    BigInt b = 0;
    BigInt c = -1;
    BigInt d = BigInt("123456789012345678901234567890");
    BigInt e = BigInt("-123456789012345678901234567890");
    
    if (a > b && b >= c && c <= a && d != b && e < d && a > e) {
        std::cout << "Comparison tests passed" << std::endl;
        return 0;
    }
    std::cout << "Comparison tests failed" << std::endl;
    return 0;
}

int
invalid_tests()
{
    try {
        BigInt a = BigInt("asdgrkeml43572");
        std::cout << "Tests with invalid arguments failed" << std::endl;
        return 1;
    }
    catch(std::exception) {}
    try {
        BigInt b = BigInt();
        b.resize(0);
        std::cout << "Tests with invalid arguments failed" << std::endl;
        return 1;
    }
    catch(std::exception) {
        std::cout << "Tests with invalid arguments passed" << std::endl;
        return 0;
    }
    return 0;
}

int
other_tests()
{
    BigInt x("123456789012345678901234567890");
    BigInt y("-123456789012345678901234567890");
    std::stringstream out1;
    std::stringstream out2;
    out1 << x;
    out2 << y;
    std::string str = out1.str();
    if (str != "123456789012345678901234567890") {
        std::cout << str << " " <<  "Print test failed" << std::endl;
        return 1;
    }
    str = out2.str();
    if (str != "-123456789012345678901234567890") {
        std::cout <<str << " " << "Print test failed" << std::endl;
        return 1;
    }
    
    std::cout << "Print test passed" << std::endl;
    return 0;
}

int
main() {
    std::cout << BigInt("0");
    std::cout << std::endl;
    int tests_failed = 0;
    tests_failed += construct_tests();
    tests_failed += math_tests();
    tests_failed += comp_tests();
    tests_failed += invalid_tests();
    tests_failed += other_tests();
    if (tests_failed) {
        std::cout << std::endl << "Tests failed" << std::endl;
    } else {
        std::cout << std::endl << "Tests passed" << std::endl; 
    }
}