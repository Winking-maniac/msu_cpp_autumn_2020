#include <vector>
#include <iostream>
#include <memory>

class Array
{
    public:
        Array();                                        //done
        Array(std::vector<unsigned> shape);             //done
        // Array(const Array& x);                          //done
        Array& operator=(const int x);                  //done
        Array& operator=(const Array& x);               //done
        std::vector<unsigned> get_shape();              //done
        unsigned get_dim();                             //done
        Array& operator*=(const int x);                 //done
        Array& operator-=(const int x);                 //done
        Array& operator-=(const Array& x);              //done
        Array& operator+=(const int x);                 //done
        Array& operator+=(const Array& x);              //done
        Array& operator+(const Array& x) const;         //done
        Array& operator+(const int x) const;            //done
        friend Array& operator+(const int x, const Array& y);  //done
        Array& operator-(const Array& x) const;         //done
        Array& operator-(const int x) const;            //done
        friend Array& operator-(const int x, const Array& y);  //done
        Array& operator*(const int x) const;            //done
        friend Array& operator*(const int x, const Array& y);  //done
        Array& operator[](int index) const;                   //done
        explicit operator int();                                 //done
        bool operator==(const Array& x) const;          //done
        bool operator!=(const Array& x) const;          //done
        friend std::ostream &operator<<(std::ostream& out, const Array& x); //done
        Array &deep_copy() const;                             //done
        Array &T();                                     //done
        unsigned rows() const;
        unsigned cols() const;
        unsigned slices() const;
    private:
        std::shared_ptr<int> data;
        std::vector<unsigned> shape;
        unsigned dim;
        unsigned offset;
        bool full_data;
        std::vector<int> strides;
        mutable std::vector<unsigned> iter;
        
        void iter_init() const;                               //done
        int &next();                                    //done
        int &next() const;                                    //done
        
};