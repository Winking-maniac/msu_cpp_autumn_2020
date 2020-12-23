// serializer.h
#pragma once
#include <iostream>
#include <typeinfo>

enum class Error
{
    NoError,
    CorruptedArchive
};

enum {
    DELIMITER = ' '
};

// template <class T>
// struct is_bool
// {
    // static constexpr bool value = false;
// };
        
// template <>
// struct is_bool<bool>
// {
    // static constexpr bool value = true;
// };

// template <class T>
// struct is_uint64
// {
    // static constexpr bool value = false;
// };
        
// template <>
// struct is_uint64<uint64_t>
// {
    // static constexpr bool value = true;
// };



class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream &out);

    template <class T> Error save(T& object);
    
    template <class... ArgsT>Error operator()(ArgsT&&... args);
    
private:
    std::ostream *out_;
    template <class T>
    Error process(T x);
    Error process(bool x);
    Error process(uint64_t x);
    
    template <class T, class... ArgsT>
    Error process(T &&x, ArgsT&&... args);
    // process использует variadic templates
};

Serializer::Serializer(std::ostream &out): out_(&out) {};

template <class T>
Error Serializer::save(T& object)
{
    return object.serialize(*this);
}
    
template <class... ArgsT>
Error Serializer::operator()(ArgsT&&... args)
{
    return process(std::forward<ArgsT>(args)...);
}


// template <>
Error Serializer::process(bool x) {
    *out_ << (x ? "true" : "false") << Serializer::Separator;
    return Error::NoError;
}

// template <>
Error Serializer::process(uint64_t x) {
    *out_ << x << Serializer::Separator;
    return Error::NoError;
}

template <class T>
Error Serializer::process(T x) {
    return Error::CorruptedArchive;
}
    
template <class T, class... ArgsT>
Error Serializer::process(T &&x, ArgsT&&... args)
{
    // std::cout << typeid(x).name();
    // fflush(stdout);
    if (process(x) != Error::NoError) {
        return Error::CorruptedArchive;
    }
    return process(std::forward<ArgsT>(args)...);
}


class Deserializer
{
    // static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream &in);

    template <class T>
    Error load(T& object);
    
    template <class... ArgsT>
    Error operator()(ArgsT&&... args);
    
private:
    std::istream *in_;
    template <class T>
    Error process(T &x);
    
    template <class T, class... ArgsT>
    Error process(T &&x, ArgsT&&... args);
    Error process(bool& x);
    Error process(uint64_t& x);
    
    // process использует variadic templates
};
   
Deserializer::Deserializer(std::istream &in): in_(&in) {};

template <class T>
Error Deserializer::load(T& object)
{
    return object.deserialize(*this);
}
    
template <class... ArgsT>
Error Deserializer::operator()(ArgsT&&... args)
{
    return process(std::forward<ArgsT>(args)...);
}

// template <>
Error Deserializer::process(bool& x) {
    std::string tmp;
    *in_ >> tmp;
    
    if ("true" == tmp) {
        x = true;
    } else if ("false" == tmp) {
        x = false;
    } else return Error::CorruptedArchive;
    return Error::NoError;
}

// template <>
Error Deserializer::process(uint64_t& x) {
    std::string tmp;
    *in_ >> tmp;
    
    try {
        x = std::stoull(tmp); 
    } 
    catch (const std::invalid_argument&) {
        return Error::CorruptedArchive;
    }
    catch (const std::out_of_range&) {
       return Error::CorruptedArchive;
    }
    return Error::NoError;
}



template <class T>
Error Deserializer::process(T &x) {
    return Error::CorruptedArchive;
}
    
template <class T, class... ArgsT>
Error Deserializer::process(T &&x, ArgsT&&... args)
{
    if (process(x) != Error::NoError) {
        return Error::CorruptedArchive;
    }
    return process(std::forward<ArgsT>(args)...);   
}
    
    