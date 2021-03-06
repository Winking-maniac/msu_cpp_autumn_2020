#include "parser.h"
#include <iostream>

void 
nop(void)
{
    ;
}

TokenParser :: TokenParser() : number_void_callback(nop), word_void_callback(nop), start_callback(nop), end_callback(nop),
        number_callback(N_VOID), word_callback(W_VOID) {}

TokenParser :: ~TokenParser() {}

void
TokenParser :: set_start_callback(VoidCallback f) 
{
    start_callback = f;
}

void
TokenParser :: set_number_token_callback(VoidCallback f)
{
    number_void_callback = f;
    number_callback = N_VOID;
}
        
void
TokenParser :: set_number_token_callback(IntCallback f)
{
    number_int_callback = f;
    number_callback = N_INT;
}

void
TokenParser :: set_number_token_callback(CharCallback f)
{
    number_char_callback = f;
    number_callback = N_CHAR;
}

void
TokenParser :: set_number_token_callback(StringCallback f)
{
    number_string_callback = f;
    number_callback = N_STRING;
}

void
TokenParser :: set_word_token_callback(VoidCallback f)
{
    word_void_callback = f;
    word_callback = W_VOID;
}

void
TokenParser :: set_word_token_callback(CharCallback f)
{
    word_char_callback = f;
    word_callback = W_CHAR;
}

void
TokenParser :: set_word_token_callback(StringCallback f)
{
    word_string_callback = f;
    word_callback = W_STRING;
}

void
TokenParser :: set_end_callback(VoidCallback f)
{
    end_callback = f;
}

void
TokenParser :: try_callback(VoidCallback f, const char* err_str, const std::string& token)
{
    try {
        f();
    }
    catch (const std::exception& err) {
        print_error(err, err_str, token);
    }
};

template <typename T>
void
TokenParser :: try_callback(std::function<void(T)>& f, T x, const char* err_str, const std::string& token)
{
    try {
        f(x);
    }
    catch (const std::exception& err) {
        print_error(err, err_str, token);
    }
};

void
TokenParser :: try_callback(StringCallback f, const std::string& x, const char* err_str, const std::string& token)
{  
    try {
        f(x);
    }
    catch (const std::exception& err) {
        print_error(err, err_str, token);
    }
};

void
TokenParser :: parse(const std::string &raw_str)
{
    try_callback(start_callback, "start_callback");
    std::string str = raw_str + ' ';
    unsigned i = 0, prev = SPACE;
    std::string cur_token = std::string();
    while (i < str.length()) {
        if (isspace(str[i])) {
            if (prev == CHAR) {
                try {
                    int number = std::stoi(cur_token);
                    if (cur_token[0] != '-' && cur_token[0] != '+' && !isdigit(cur_token[0]))
                        throw std::invalid_argument("");
                    for (unsigned j = 1; j < cur_token.length(); j++) {
                        if (!isdigit(cur_token[j]))
                            throw std::invalid_argument("");
                        //std::cout << cur_token[j] << std::endl << isdigit(cur_token[j]) << std::endl;
                    }
                    if (number_callback == N_VOID) {
                        try_callback(number_void_callback, "number_void_callback", cur_token);
                    } else if (number_callback == N_INT) {
                        try_callback(number_int_callback, number, "number_int_callback", cur_token);
                    } else if (number_callback == N_STRING) {
                        try_callback(StringCallback(number_string_callback), static_cast<const std::string &>(cur_token), "number_string_callback", cur_token);
                    } else if (number_callback == N_CHAR) {
                        try_callback(number_char_callback, cur_token.c_str(), "number_char_callback", cur_token);
                    } 
                }
                catch(std::invalid_argument) {
                    if (word_callback == W_VOID) {
                        try_callback(word_void_callback, "word_void_callback", cur_token);
                    } else if (word_callback == W_STRING) {
                        try_callback(word_string_callback, static_cast<const std::string &>(cur_token), "number_string_callback", cur_token);
                    } else if (word_callback == W_CHAR) {
                        try_callback(word_char_callback, cur_token.c_str(), "word_char_callback", cur_token);
                    }
                }
                cur_token = std::string();
            } 
            prev = SPACE;
        } else {
            cur_token.push_back(str[i]);
            prev = CHAR;
        }
        i++;
    }
    try_callback(end_callback, "end_callback");
}

void
TokenParser :: parse(const char *str)
{
    if (str == nullptr) return;
    parse(std::string(str));
}

void
TokenParser :: print_error(const std::exception& err, const char* where, const std::string& cur_token)
{
    std::cout << "In TokenParser: " << where << " exception" << std::endl;
    std::cout << "\t" << err.what() << std::endl;
    if (cur_token != std::string())
        std::cout << "\tToken: " << cur_token<< std::endl;
}
