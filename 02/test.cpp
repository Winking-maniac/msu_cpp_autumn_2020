#include "parser.h"
#include <iostream>

char test[] = "Hello world 123 qcgdl;w.980\t980123\nasd 123qwer";

void
test_uninitialized()
{
    std::cout << "Uninitialized parser test started" << std::endl;
    TokenParser x;
    x.parse(test);
    std::cout << "\tNothing happened - correct" << std::endl << std::endl;
}

void
n_void()
{
    std::cout << "\tNumber token from void" << std::endl;
}

void
test_number_void()
{
    std::cout << "Void number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_void);
    x.parse(test);
    
    std::cout << "\tCorrect output: 2 numbers" << std::endl << std::endl;
}

void
n_int(int x)
{
    std::cout << "\tNumber token from int - " << x << std::endl;
}

void
test_number_int()
{
    std::cout << "Int number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_int);
    x.parse(test);
    
    std::cout << "\tCorrect output: 123, 980123" << std::endl << std::endl;
}

void
n_char(const char *x)
{
    std::cout << "\tNumber token from char* - " << x << std::endl;
}

void
test_number_char()
{
    std::cout << "Char* number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_char);
    x.parse(test);
    std::cout << "\tCorrect output: 123, 980123" << std::endl << std::endl;
}

void
n_string(const std::string x)
{
    std::cout << "\tNumber token from string - " << x << std::endl;
}

void
test_number_string()
{
    std::cout << "String number token callback test started" << std::endl;
    TokenParser x;
    x.set_number_token_callback(n_string);
    x.parse(test);
    std::cout << "\tCorrect output: 123, 980123" << std::endl << std::endl;
}

void
w_void()
{
    std::cout << "\tWord token from void" << std::endl;
}

void
test_word_void()
{
    std::cout << "Void word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(w_void);
    x.parse(test);
    
    std::cout << "\tCorrect output: 5 words" << std::endl << std::endl;
}

void
w_char(const char *x)
{
    std::cout << "\tWord token from char* - " << x << std::endl;
}

void
test_word_char()
{
    std::cout << "Char* word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(w_char);
    x.parse(test);
    std::cout << "\tCorrect output: Hello, world, qcgdl;w.980, asd, 123qwer" << std::endl << std::endl;
}

void
w_string(const std::string x)
{
    std::cout << "\tWord token from string - " << x << std::endl;
}

void
test_word_string()
{
    std::cout << "String word token callback test started" << std::endl;
    TokenParser x;
    x.set_word_token_callback(w_string);
    x.parse(test);
    std::cout << "\tCorrect output: Hello, world, qcgdl;w.980, asd, 123qwer" << std::endl << std::endl;
}

void
st()
{
    std::cout << "\tStart callback successful" << std::endl << std::endl;
}

void
test_start()
{
    std::cout << "Start callback test started" << std::endl;
    TokenParser x;
    x.set_start_callback(st);
    x.parse(test);
}

void
end()
{
    std::cout << "\tEnd callback successful" << std::endl << std::endl;
}

void
test_end()
{
    std::cout << "End callback test started" << std::endl;
    TokenParser x;
    x.set_end_callback(end);
    x.parse(test);
}

void
st_last()
{
    std::cout << "\tStart callback successful (last)" << std::endl;
}

void
end_last()
{
    std::cout << "\tEnd callback successful (last)" << std::endl << std::endl;
}

void
w_char_last(const char *x)
{
    std::cout << "\tWord token from char* - " << x  << " (last)"<< std::endl;
}

void
n_char_last(const char* x)
{
    std::cout << "\tNumber token from char* - " << x << " (last)"<< std::endl;
}

void
test_multiple_init()
{
    std::cout << "Multiple callback init test started - all outputs should have suffix (last)" << std::endl<< std::endl;
    TokenParser x;
    x.set_start_callback(st);
    x.set_start_callback(st);
    x.set_start_callback(st_last);
    
    x.set_end_callback(end);
    x.set_end_callback(end);
    x.set_end_callback(end_last);
    
    x.set_number_token_callback(n_string);
    x.set_number_token_callback(n_char);
    x.set_number_token_callback(n_string);
    x.set_number_token_callback(n_int);
    x.set_number_token_callback(n_char_last);
    
    x.set_word_token_callback(w_string);
    x.set_word_token_callback(w_char);
    x.set_word_token_callback(w_string);
    x.set_word_token_callback(w_char_last);
    
    x.parse("Hello world 123 qcgdl;w.980\t980123\nasd");
}

int main() {
    std::cout << std::endl << std::endl << std::endl << "TEST"
            << std::endl << std::endl << std::endl << std::endl;
    test_uninitialized();
    test_number_void();
    test_number_int();
    test_number_char();
    test_number_string();
    test_word_void();
    test_word_char();
    test_word_string();
    test_start();
    test_end();
    test_multiple_init();
    return 0;
}