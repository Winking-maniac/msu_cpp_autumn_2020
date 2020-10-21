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
TokenParser :: parse(const std::string raw_str)
{
    try {
        start_callback();
    }
    catch (std::exception err) {
        print_error(err, "start_callback");
    }
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
                        try {
                            number_void_callback();
                        }
                        catch(std::exception err) {
                            print_error(err, "number_void_callback", cur_token);
                        }
                    } else if (number_callback == N_INT) {
                        try {
                            number_int_callback(number);
                        }
                        catch(std::exception err) {
                            print_error(err, "number_int_callback", cur_token);
                        }
                    } else if (number_callback == N_STRING) {
                        try {
                            number_string_callback(cur_token);
                        }
                        catch(std::exception err) {
                            print_error(err, "number_string_callback", cur_token);
                        }
                    } else if (number_callback == N_CHAR) {
                        try {
                            number_char_callback(cur_token.c_str());
                        }
                        catch(std::exception err) {
                            print_error(err, "number_char_callback", cur_token);
                        }
                    } 
                }
                catch(std::invalid_argument) {
                    if (word_callback == W_VOID) {
                        try {
                            word_void_callback();
                        }
                        catch(std::exception err) {
                            print_error(err, "word_void_callback", cur_token);
                        }
                    } else if (word_callback == W_STRING) {
                        try {
                            word_string_callback(cur_token);
                        }
                        catch(std::exception err) {
                            print_error(err, "word_string_callback", cur_token);
                        }
                    } else if (word_callback == W_CHAR) {
                        try {
                            word_char_callback(cur_token.c_str());
                        }
                        catch(std::exception err) {
                            print_error(err, "word_char_callback", cur_token);
                        }
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
     
    
    try {
        end_callback();
    }
    catch (std::exception err) {
        print_error(err, "end_callback");
    }
}

void
TokenParser :: parse(const char *str)
{
    if (str == nullptr) return;
    parse(std::string(str));
}

void
TokenParser :: print_error(std::exception err, const char* where, std::string cur_token)
{
    std::cout << "In TokenParser: " << where << " exception" << std::endl;
    std::cout << "\t" << err.what() << std::endl;
    if (cur_token != std::string())
        std::cout << "\tToken: " << cur_token<< std::endl;
}
