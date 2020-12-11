#include "parser.h"
#include <iostream>

char test[] = "Hello world 123 qcgdl;w.980\t980123\nasd 123qwer";
int i[3] = {0}; // start/end  number  word
std::string tokens = std::string();

void nullify_i() {i[0] = 0, i[1] = 0, i[2] = 0, tokens = std::string();}

int
test_uninitialized()
{
    nullify_i();
    std::cout << "Uninitialized parser test started" << std::endl;
    TokenParser x;
    x.parse(test);
    if (i[0] == 0 && i[1] == 0 && i[2] == 0) {
        return 0;
    }
    return 1;
}

void
n_void()
{
    i[1]++;
}

int
test_number_void()
{
    nullify_i();
    std::cout << "Void number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_void);
    x.parse(test);
    if (i[0] == 0 && i[1] == 2 && i[2] == 0) {
        return 0;
    }
    return 1;
}

void
n_int(int x)
{
    i[1]++;
    tokens += std::to_string(x);
    tokens += "\n";
}

int
test_number_int()
{
    nullify_i();
    std::cout << "Int number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_int);
    x.parse(test);
    if (i[0] == 0 && i[1] == 2 && i[2] == 0 && tokens == "123\n980123\n") {
        return 0;
    }
    return 1;
}

void
n_char(const char *x)
{
    i[1]++;
    tokens += x;
    tokens += "\n";
}

int
test_number_char()
{
    nullify_i();
    std::cout << "Char* number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_char);
    x.parse(test);
    if (i[0] == 0 && i[1] == 2 && i[2] == 0 && tokens == "123\n980123\n") {
        return 0;
    }
    return 1;
}

void
n_string(const std::string &x)
{
    i[1]++;
    tokens += x;
    tokens += "\n";
}

int
test_number_string()
{
    nullify_i();
    std::cout << "String number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(StringCallback(n_string));
    x.parse(test);
    if (i[0] == 0 && i[1] == 2 && i[2] == 0 && tokens == "123\n980123\n") {
        return 0;
    }
    return 1;
}

void
w_void()
{
    i[2]++;
}

int
test_word_void()
{
    nullify_i();
    std::cout << "Void word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(w_void);
    x.parse(test);
    if (i[0] == 0 && i[1] == 0 && i[2] == 5) {
        return 0;
    }
    return 1;
}

void
w_char(const char *x)
{
    i[2]++;
    tokens += x;
    tokens += "\n";
}

int
test_word_char()
{
    nullify_i();
    std::cout << "Char* word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(w_char);
    x.parse(test);
    if (i[0] == 0 && i[1] == 0 && i[2] == 5 && tokens == "Hello\nworld\nqcgdl;w.980\nasd\n123qwer\n") {
        return 0;
    }
    return 1;
    // std::cout << "\tCorrect output: Hello, world, qcgdl;w.980, asd, 123qwer" << std::endl << std::endl;
}

void
w_string(const std::string x)
{
    i[2]++;
    tokens += x;
    tokens += "\n";
}

int
test_word_string()
{
    nullify_i();
    std::cout << "String word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(StringCallback(w_string));
    x.parse(test);
    
    if (i[0] == 0 && i[1] == 0 && i[2] == 5 && tokens == "Hello\nworld\nqcgdl;w.980\nasd\n123qwer\n") {
        return 0;
    }
    return 1;
    // std::cout << "\tCorrect output: Hello, world, qcgdl;w.980, asd, 123qwer" << std::endl << std::endl;
}

void
st()
{
    i[0]++;
}

int
test_start()
{
    nullify_i();
    std::cout << "Start callback test started" << std::endl;
    TokenParser x;
    x.set_start_callback(st);
    x.parse(test);
    if (i[0] == 1 && i[1] == 0 && i[2] == 0) {
        return 0;
    }
    return 1;
}

void
end()
{
    i[0]++;
}

int
test_end()
{
    nullify_i();
    std::cout << "End callback test started" << std::endl;
    TokenParser x;
    x.set_end_callback(end);
    x.parse(test);
    if (i[0] == 1 && i[1] == 0 && i[2] == 0) {
        return 0;
    }
    return 1;
}

void
st_last()
{
    i[0]--;
}

void
end_last()
{
    i[0]--;
}

void
w_char_last(const char *x)
{
    i[2]--;
}

void
n_char_last(const char* x)
{
    i[1]--;
}

int
test_multiple_init()
{
    nullify_i();
    std::cout << "Multiple callback init test started" << std::endl<< std::endl;
    TokenParser x;
    x.set_start_callback(st);
    x.set_start_callback(st);
    x.set_start_callback(st_last);
    
    x.set_end_callback(end);
    x.set_end_callback(end);
    x.set_end_callback(end_last);
    
    x.set_number_token_callback(StringCallback(n_string));
    x.set_number_token_callback(n_char);
    x.set_number_token_callback(StringCallback(n_string));
    x.set_number_token_callback(n_int);
    x.set_number_token_callback(n_char_last);
    
    x.set_word_token_callback(StringCallback(w_string));
    x.set_word_token_callback(w_char);
    x.set_word_token_callback(StringCallback(w_string));
    x.set_word_token_callback(w_char_last);
    
    x.parse("Hello world 123 qcgdl;w.980\t980123\nasd");
    if (i[0] == -2 && i[1] == -2 && i[2] == -4) {
        return 0;
    }
    return 1;
}

int main() {
    std::cout << std::endl << std::endl << std::endl << "TESTING"
            << std::endl << std::endl << std::endl << std::endl;
    int tests_failed = 0;
    tests_failed += test_uninitialized();
    tests_failed += test_number_void();
    tests_failed += test_number_int();
    tests_failed += test_number_char();
    tests_failed += test_number_string();
    tests_failed += test_word_void();
    tests_failed += test_word_char();
    tests_failed += test_word_string();
    tests_failed += test_start();
    tests_failed += test_end();
    tests_failed += test_multiple_init();
    
    if (tests_failed) {
        std::cout << tests_failed << " out of 11 tests failed\n\n";
    } else {
        std::cout << "All tests passed\n\n";
    }
    return 0;
}