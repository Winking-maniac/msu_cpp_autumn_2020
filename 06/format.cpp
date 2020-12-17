#include <ctype.h>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <exception>
#include <stdexcept>

class FormatException: public std::exception
{
    private:
        std::string start = std::string("Invalid format string - ");
    public:
        FormatException(const char* what){}
        const char* what() const noexcept
        {
            return (start + ((static_cast<std::exception>(*this)).what())).c_str();
        }
};

class MissingArgumentException: public std::exception
{    
    public:
        MissingArgumentException(const char* what){}
     
        const char* what() const noexcept
        {
            return (static_cast<std::exception>(*this)).what();
        }
};

template <class T, class... ArgsT>
static std::vector<std::string>
args2str(const T &x) {
    std::vector<std::string> res;
    std::stringstream s;
    s << x;
    // std::cout << res->size();
    res.push_back(s.str());
    return res;
}

template <class T, class... ArgsT>
static std::vector<std::string>
args2str(const T &x, const ArgsT&... args) {
    std::vector<std::string> res = args2str(args...);
    std::stringstream s;
    s << x;
    // std::cout << res.size();
    res.push_back(s.str());
    return res;
}

template <class... ArgsT>
std::string
format(const std::string &format_string, const ArgsT&... args)
{
    long long i = 0, cur_num = 0, max_num = 0;
    bool is_number = false;
    while(format_string[i]) {
        if ((format_string[i] == '{' && is_number) || 
                (format_string[i] == '}' && !is_number)){
            throw FormatException("unexpected { or }");
        }
        if (is_number && format_string[i] != '}') {
            if (isdigit(format_string[i])) {
                cur_num = 10 * cur_num + format_string[i] - '0';
            } else {
                throw FormatException("invalid argument number");
            }
        } else if (format_string[i] == '{') {
            if (format_string[i+1] == '}') {
                throw FormatException("missing argument number");
            }
            is_number = true;
        } else if (format_string[i] == '}') {
            is_number = false;
            if (cur_num < 0) {
                throw FormatException("too many arguments");
            }
            if (cur_num > max_num) {
                max_num = cur_num;
            }
            cur_num = 0;
        }
        i++;
    }
    if (is_number) {
        throw FormatException("missing } at the end of format string");
    }
    std::vector<std::string> str = args2str(args...);
    std::reverse(str.begin(), str.end()); 
    std::stringstream out;
    
    if (max_num >= static_cast<signed>(str.size())) {
        throw MissingArgumentException("Too few function arguments");
    }
    i = 0;
    while(format_string[i]) {
        if (is_number && format_string[i] != '}') {
            if (isdigit(format_string[i])) {
                cur_num = 10 * cur_num  + format_string[i] - '0';
            }
        } else if (format_string[i] == '{') {
            is_number = true;
        } else if (format_string[i] == '}') {
            is_number = false;
            if ((unsigned long long)cur_num < str.size()) {
                out << str[cur_num];
            }
            
            cur_num = 0;
        } else {
            out << format_string[i];
        }
        i++;
        
    }
    return std::string(out.str()); 
}

int main()
{
    //Standart test
    auto text = format("{1}+{1} = {0}", 2, "one");
    assert(text == "one+one = 2");
    //Missing argument in format string
    text = format("{2}+{2} = {0}", 2, "one", "zero");
    assert(text == "zero+zero = 2");
    
    //Missing argument
    int i = 0;
    
    try
    {
       text = format("{2}+{2} = {0}", 2, "one");
       i++;
    }
    catch(MissingArgumentException) {
        ;
    }
    
    //Incorrect format strings
    try
    {
        text = format(std::string("{1+}+{1} = {0}"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    try
    {
        text = format(std::string("{1}+{1} = {0"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    try
    {
        text = format(std::string("{}{1}+{1} = {0}"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    try
    {
        text = format(std::string("{1{}+{1} = {0}"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    try
    {
        text = format(std::string("{1}}+{1} = {0}"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    try
    {
        text = format(std::string("{10000000000000000000}+{1} = {0}"), 2, "one");
        i++;
    }
    catch (FormatException) {
        ;
    }
    
    if (i == 0) {
        std::cout << "All tests passed" <<std::endl;
    } else {
        std::cout << i << " incorrect format string tests failed" <<std::endl;
    }
    return 0;
}