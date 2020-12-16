#ifndef BIGINT_H
#define BIGINT_H
#include <cstdint>
#include <iostream>


class BigInt
{
    private:
        BigInt operator*(std::pair<std::uint32_t, int> x) const;                                  //done
        int divmod10();                                                                 //done            
        
        std::uint32_t* integer;
        std::uint64_t size;
        bool sign;
    public:
        // Constructors, destructor
        BigInt(const std::string &str_value);                                           //done
        BigInt(int value);                                                              //done                
        explicit BigInt();                                                              //done
        
        /*BigInt& */BigInt(const BigInt& x);                                            //done
        BigInt(BigInt&& x) noexcept;                                                    //done
        
        BigInt& operator=(const BigInt& x);                                             //done
        BigInt& operator=(BigInt&& x) noexcept;                                         //done
        
        ~BigInt() noexcept;                                                             //done
        
        // Math operators
        
        
        BigInt operator-() const;                                                       //done
        
        BigInt& operator+=(const int x);                                                //done    
        BigInt& operator+=(const BigInt& x);                                            //done
        BigInt operator+(const BigInt& x) const;                                        //done
        BigInt operator+(const int x) const;                                            //done
        friend BigInt operator+(const int x, const BigInt& y);                          //done
       
        BigInt& operator-=(const int x);                                                //done
        BigInt& operator-=(const BigInt& x);                                            //done
        BigInt operator-(const BigInt& x) const;                                        //done
        BigInt operator-(const int x) const;                                            //done
        friend BigInt operator-(const int x, const BigInt& y);                          //done
        
        BigInt& operator*=(int x);                                                      //done   
        BigInt& operator*=(const BigInt& x);                                            //done
        BigInt operator*(const int x) const;                                            //done
        BigInt operator*(const BigInt& x) const;                                        //done
        friend BigInt operator*(const int x, const BigInt& y);                          //done
        
        // Comparison operators
        
        bool operator==(const BigInt& x) const noexcept;                                //done
        bool operator!=(const BigInt& x) const noexcept;                                //done
        bool operator>=(const BigInt& x) const noexcept;                                //done
        bool operator<=(const BigInt& x) const noexcept;                                //done
        bool operator>(const BigInt& x) const noexcept;                                 //done
        bool operator<(const BigInt& x) const noexcept;                                 //done 
        
        // Other
        friend std::ostream &operator<<(std::ostream& out, const BigInt& x);            //done
        void resize(std::uint64_t new_size);                                            //done    
        void shrink();                                                                  //done
    
};

#endif