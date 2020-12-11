#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <exception>
#include <stdexcept>
#include <functional>

using IntCallback = std::function<void(int)>;
using CharCallback = std::function<void(const char*)>;
using StringCallback = std::function<void(const std::string &)>;
using VoidCallback = std::function<void(void)>;
// typedef void(*CharCallback)(const char *);
// typedef void(*StringCallback)(const std::string &);
// typedef void(*VoidCallback)(void);

enum
{
    N_VOID,
    N_INT,
    N_CHAR,
    N_STRING,
    W_VOID,
    W_CHAR,
    W_STRING, 
    SPACE,
    CHAR
};


void 
nop(void);

class TokenParser
{
    private:
        VoidCallback number_void_callback;
        VoidCallback word_void_callback;
        IntCallback number_int_callback;
        CharCallback number_char_callback;
        CharCallback word_char_callback;
        StringCallback number_string_callback;
        StringCallback word_string_callback;
        
        VoidCallback start_callback;
        VoidCallback end_callback;
        
        int number_callback;
        int word_callback;
        
        void print_error(std::exception& err, const char* where, const std::string& cur_token = std::string());
        
        void try_callback(VoidCallback f, const char* err_str, const std::string& token = std::string());
        
        template <class T>
        void try_callback(std::function<void(T)> &f, T x, const char* err_str, const std::string& token);
        
        void try_callback(StringCallback f, const std::string& x, const char* err_str, const std::string& token);
        
    public:
        TokenParser();       
        ~TokenParser();
        
        void set_start_callback(VoidCallback f);
        
        void set_number_token_callback(VoidCallback f);
        
        void set_number_token_callback(IntCallback f);
        
        void set_number_token_callback(CharCallback f);
        
        void set_number_token_callback(StringCallback f);
        
        void set_word_token_callback(VoidCallback f);
        
        void set_word_token_callback(CharCallback f);
        
        void set_word_token_callback(StringCallback f);
        
        void set_end_callback(VoidCallback f);
        
        void parse(const std::string raw_str);
        
        void parse(const char *str);
        
};


#endif