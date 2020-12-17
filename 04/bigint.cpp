#include "bigint.h"
#include <stdexcept>
#include <iomanip>
#include <cstring>
#include <new>

enum { MAX_BASE_POWER = 1000000000 };

// Constructors, destructor

BigInt::BigInt() : size(1), sign(false)
{
    integer = static_cast<std::uint32_t*>(calloc(1, sizeof(*integer)));
    if(integer == nullptr) {   
        throw std::bad_alloc();
    }
}

BigInt::BigInt(int value) : size(1), sign(false)
{
    if (value < 0) {
        value = -value;
        sign = true;
    }
    if (value > MAX_BASE_POWER) {
        integer = static_cast<std::uint32_t*>(malloc(2 * sizeof(*integer)));
    } else {
        integer = static_cast<std::uint32_t*>(malloc(sizeof(*integer)));
    }
    if(integer == nullptr) {   
        throw std::bad_alloc();
    }
    integer[0] = value % MAX_BASE_POWER;
    if (value/MAX_BASE_POWER) {
        integer[1] = value/MAX_BASE_POWER;
    }
}

BigInt::BigInt(const std::string &str_value)
{
    *this = BigInt();
    bool tmp_sign = false;
    std::string::const_iterator it = str_value.cbegin();
    if (str_value[0] == '-') {
        tmp_sign = true;
        it++;
    } else if (str_value[0] == '+') {
        it++;
    }
    for (; it != str_value.cend(); it++) {
        *this *= 10;
        if (*it < '0' || *it > '9') {
            throw std::invalid_argument("BigInt: cannot convert into number");
        } else {
            *this += (*it - '0');
        }
    }
    
    if (tmp_sign) {
        this->sign = true;
    }
    if (*this == 0) {
        this->sign = false;
    }
    
}

BigInt::BigInt(const BigInt& x) : size(x.size), sign(x.sign)
{
    integer = static_cast<std::uint32_t*>(malloc(size * sizeof(*integer)));
    if(integer == nullptr) {   
        throw std::bad_alloc();
    }
    memcpy(integer, x.integer, size * sizeof(*integer));
}

BigInt::BigInt(BigInt&& x) noexcept : integer(x.integer), size(x.size), sign(x.sign)
{
    x.size = 0;
    x.integer = nullptr;
    x.sign = false;
}

BigInt&
BigInt::operator=(const BigInt& x)
{
    this->size = x.size;
    this->sign = x.sign;
    this->integer = static_cast<std::uint32_t*>(malloc(this->size * sizeof(*(this->integer))));
    if(this->integer == nullptr) {   
        throw std::bad_alloc();
    }
    memcpy(this->integer, x.integer, this->size * sizeof(*integer));
    return *this;
}

BigInt& 
BigInt::operator=(BigInt&& x) noexcept
{
    this->size = x.size;
    this->sign = x.sign;
    this->integer = x.integer;
    x.size = 0;
    x.size = false;
    x.integer = nullptr;
    return *this;
}

BigInt::~BigInt() noexcept
{
    free(integer);
}

// Math operators

BigInt
BigInt::operator-() const
{
    BigInt res = *this;
    if (*this != 0) res.sign = !res.sign;
    return res;
}

BigInt&
BigInt::operator+=(const int x) {
    *this += BigInt(x);
    return *this;
}

BigInt&
BigInt::operator+=(const BigInt& x) {
    if (x == 0) return *this;
    if (x.sign != this->sign) {
        return *this -= -x;
    }
    if (x.size > this->size) {
        this->resize(x.size);
    }
    
    bool cf = false;
    for (unsigned i = 0; i < x.size; i++) {
        this->integer[i] += x.integer[i] + cf;
        cf = this->integer[i] / MAX_BASE_POWER;
        this->integer[i] %= MAX_BASE_POWER;
    }
    for (unsigned i = x.size; i < this->size; i++) {
        this->integer[i] += cf;
        cf = this->integer[i] / MAX_BASE_POWER;
        this->integer[i] %= MAX_BASE_POWER;
    }
    if (cf) {
        this->resize(this->size+1);
        this->integer[this->size-1] = cf;
    }
    return *this;
}

BigInt
BigInt::operator+(const BigInt& x) const
{
    BigInt *res = new BigInt(*this);
    return *res += x;
}

BigInt
BigInt::operator+(const int x) const
{
    BigInt *res = new BigInt(*this);
    return *res += x;
}

BigInt
operator+(const int x, const BigInt& y)
{
    return y + x;
} 

BigInt&
BigInt::operator-=(const int x)
{
    return *this -= BigInt(x);
}

BigInt&
BigInt::operator-=(const BigInt& x)
{
    if (x == 0) return *this;
    if (x.sign != this->sign) {
        return *this += -x;
    }
    bool this_greater = false;;
    if (x.size > this->size) {
        this->resize(x.size);
    }
    for (unsigned i = this->size - 1; i >= x.size; i--) {
        if (this->integer[i]) {
            this_greater = true;
            break;
        }
    }
    if (!this_greater) {
        for (unsigned i = x.size - 1; i >= 0; i--) {
            if (this->integer[i] > x.integer[i]) {
                this_greater = true;
                break;
            }
            if (this->integer[i] < x.integer[i]) {
                this_greater = false;
                break;
            }
        }
    }
    
    bool cf = true;
    if (this_greater) {
        for (unsigned i = 0; i < x.size; i++) {
            this->integer[i] += MAX_BASE_POWER - x.integer[i] - 1 + cf;
            cf = this->integer[i] / MAX_BASE_POWER;
            this->integer[i] %= MAX_BASE_POWER;
        }
        for (unsigned i = x.size; i < this->size; i++) {
            this->integer[i] += MAX_BASE_POWER - 1 + cf;
            cf = this->integer[i] / MAX_BASE_POWER;
            this->integer[i] %= MAX_BASE_POWER;
        }
    } else {
        this->resize(x.size);
        for (unsigned i = 0; i < this->size; i++) {
            this->integer[i] = MAX_BASE_POWER + x.integer[i] - this->integer[i] - 1 + cf;
            cf = this->integer[i] / MAX_BASE_POWER;
            this->integer[i] %= MAX_BASE_POWER;
        }
    }
    if (!this_greater) {
        this->sign = !this->sign;
    }
    return *this;
}

BigInt
BigInt::operator-(const BigInt& x) const
{
    BigInt *res = new BigInt(*this);
    return *res -= x;
}

BigInt
BigInt::operator-(const int x) const
{
    BigInt *res = new BigInt(*this);
    return *res += -x;
}

BigInt
operator-(const int x, const BigInt& y)
{
    return -(y-x);
}  

BigInt&
BigInt::operator*=(int x)
{
    *this *= BigInt(x);
    return *this;
}                            

BigInt&
BigInt::operator*=(const BigInt& x)
{
    if (x.sign) {
        this->sign = !this->sign;
    }
    BigInt* tmp = new BigInt(0);
    for (unsigned i = 0; i < x.size; i++) {
        *tmp += *this * std::make_pair(x.integer[i], i);
    }
    *this = std::move(*tmp);
    return *this;
}

BigInt
BigInt::operator*(const BigInt& x) const
{
    BigInt *res = new BigInt(*this);
    return *res *= x;
}

BigInt operator*(const int x, const BigInt& y)
{
    return BigInt(x) * y;
}

BigInt
BigInt::operator*(const int x) const
{
    return *this * BigInt(x);
}          

// Comparison operators

bool
BigInt::operator==(const BigInt& x) const noexcept
{
    if (this->sign != x.sign) {
        return false;
    }
    if (x.size > this->size) {
        return x == *this;
    }
    for (unsigned i = 0; i < x.size; i++) {
        if (this->integer[i] != x.integer[i]) return false;
    }
    for (std::uint64_t i = x.size; i < this->size; i++) {
        if (this->integer[i] != 0) return false;
    }
    return true;
}

bool
BigInt::operator!=(const BigInt& x) const noexcept
{
    return !(*this==x);
}

bool
BigInt::operator>=(const BigInt& x) const noexcept
{
    return !(*this < x);
}

bool
BigInt::operator<=(const BigInt& x) const noexcept
{
    return !(x < *this);
}

bool
BigInt::operator>(const BigInt& x) const noexcept
{
    return x < *this;
}

bool
BigInt::operator<(const BigInt& x) const noexcept
{
    BigInt tmp = *this - x;
    return tmp.sign;
}


// Other

std::ostream &
operator<<(std::ostream& out, const BigInt &x)
{
    if (x.sign) {
        out << "-";
    }
    int i = x.size - 1;
    while (x.integer[i] == 0) i--;
    out << x.integer[i--];
    for (; i >= 0; i--) {
        out << std::setw(9) << std::setfill('0') << x.integer[i];
    }
    return out;
}

void
BigInt::resize(std::uint64_t new_size)
{
    if (new_size == 0) {
        throw std::invalid_argument("BigInt: can't resize to 0");
    }
    std::uint32_t* tmp = static_cast<std::uint32_t*>(realloc(integer, sizeof(*integer) * new_size));
    if (tmp == nullptr) {
        throw std::bad_alloc();
    }
    if (new_size > size) memset(tmp + size, 0, sizeof(*integer)*(new_size - size));
    integer = static_cast<std::uint32_t*>(tmp);
    size = new_size;
}

void
BigInt::shrink()
{
    int new_size = size;
    while(new_size > 1 && !integer[new_size - 1]) {
        new_size--;
    }
    integer = static_cast<std::uint32_t*>(realloc(integer, new_size));    
    size = new_size;
}

// Private

BigInt
BigInt::operator*(std::pair<std::uint32_t, int> x) const
{
    if (x.first == 0) return BigInt();
    BigInt res = BigInt();
    res.resize(x.second + size + 1);
    memset(res.integer, 0, sizeof(*integer) * x.second);
    std::uint64_t tmp = 0;
    for (unsigned i = 0; i < this->size; i++) {
        tmp += this->integer[i] * static_cast<std::uint64_t>(x.first);
        res.integer[i + x.second] = tmp % MAX_BASE_POWER;
        tmp /= MAX_BASE_POWER;
    }
    res.integer[x.second + this->size] = tmp;
    res.sign = this->sign; 
    return res;
}      