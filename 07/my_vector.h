#ifndef MY_VECTOR
#define MY_VECTOR

#include <cstdlib> 
#include <stdexcept> 


template <class T>
class My_allocator
{
    public:
        My_allocator() {}

        T* allocate(size_t n) {
            T* res = static_cast<T*>(malloc(sizeof(T) * n));
            if (res == nullptr) {
                throw std::bad_alloc();
            }
            return res;
        };
        
        void deallocate(T *pointer) {
            free(pointer);
        }
};

template <class T>
class My_iterator
{
    private:
        T *cur;
    public:
        My_iterator(T *cur) : cur(cur) {}
        
        My_iterator<T>& operator++() {
            cur++;
            return *this;
        }
        
        T& operator*() {
            return *cur;
        }
        
        bool operator==(const My_iterator<T> x) {
            return x.cur == cur;
        }
        
        bool operator!=(const My_iterator<T> x) {
            return x.cur != cur;
        } 
};


template <class T>
class My_reverse_iterator
{
    private:
        T *cur;
    public:
        My_reverse_iterator(T *cur) : cur(cur) {}
        
        My_reverse_iterator<T>& operator++() {
            cur--;
            return *this;
        }
        
        T& operator*() {
            return *cur;
        }
        
        bool operator==(const My_reverse_iterator<T> x) {
            return x.cur == cur;
        }
        
        bool operator!=(const My_reverse_iterator<T> x) {
            return x.cur != cur;
        }
};

template<
    class T,
    class Allocator = My_allocator<T>
> class My_vector
{
    private:
        Allocator alloc;
        size_t capacity_;
        T *first;
        T *last;
        T *end_;
    public:
        My_vector()
        {
            alloc = Allocator();
            first = last = alloc.allocate(1);
            end_ = first + 1;
        }

        ~My_vector() noexcept
        {
            T *cur = first; 
            while (cur != last) {
                cur++ -> ~T();
            }
            alloc.deallocate(first);
        }
        
        // My_vector(const My_vector<T> &x);
        // My_vector(My_vector<T> &&x);
        // My_vector& operator=(const My_vector<T> &x);
        // My_vector& operator=(const My_vector<T> &&x);
        
        T& operator[](size_t i)
        {
            if ((first + i) >= last) {
                throw std::out_of_range("");
            }
            return *(first + i);
        }

        const T& operator[](size_t i) const
        {
            if ((first + i) >= last) {
                throw std::out_of_range();
            }
            return static_cast<const T&>(*(first + i));
        }
        
        
        bool empty() const noexcept
        {
            return first == last;
        }
        
        size_t size() const noexcept
        {
            return last - first;
        }

        size_t capacity() const noexcept
        {
            return end_ - first;
        }
        
        void clear() noexcept
        {
            T *cur = first;
            while (cur != last) {
                cur++ -> ~T();
            }
            last = first;
        }

        void resize(size_t count)
        {
            if (first + count > end_) {
                this->reserve(count);
            }
            if (first + count > last) {
                while (last != first + count) {
                    new (last++) T();
                }
            } else {
                T *cur_last = last - 1;
                last = first + count;
                while (cur_last >= last) {
                    cur_last-- -> ~T();
                }
            }
        }

        void reserve(size_t new_cap)
        {
            if (first + new_cap < end_) return;
            T *new_first = alloc.allocate(new_cap);
            T *cur = new_first;
            while (first != last) {
                new (cur++) T(std::move(*(first++)));
            }
            first = new_first;
            last = cur;
            end_ = first + new_cap;
        }

        void push_back(const T& value)
        {
            if (last == end_) {
                reserve(2 * (end_ - first));
            }
            new (last++) T(value);
        }

        void push_back(T&& value)
        {
            if (last == end_) {
                reserve(2 * (end_ - first));
            }
            new (last++) T(value);
        }

        void pop_back()
        {
            last--;
            last -> ~T();
        }

        template< class... Args >
        void emplace_back( Args&&... args )
        {
            if (last == end_) {
                reserve(2 * (end_ - first));
            }
            new (last++) T(args...);
        }
        
        My_iterator<T> begin()
        {
            return My_iterator<T>(first);
        }

        My_iterator<T> end()
        {
            return My_iterator<T>(last);
        }

        My_reverse_iterator<T> rbegin()
        {
            return My_reverse_iterator<T>(last - 1);
        }

        My_reverse_iterator<T> rend()
        {
            return My_reverse_iterator<T>(first - 1);
        }
};
        
#endif