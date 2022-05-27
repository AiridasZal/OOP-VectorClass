#pragma once

#include <limits>
#include <memory>
#include <stdexcept>

template <typename T> class vector {
public:
    // Type definitions

    typedef std::allocator<T> allocator_type;
    allocator_type alloc;
    typedef T value_type;
    typedef size_t size_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef T *iterator;
    typedef const T *const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    
    // Constructor and Destructor / all

    vector() { create(); }
    explicit vector(size_type n, const value_type &value = value_type()) { create(n, value); }
    vector(const vector &other) { create(other.begin(), other.end()); }
    vector(std::initializer_list<value_type> il) { create(il.begin(), il.end()); }
    ~vector() { uncreate(); }
    
    // Iterators / done

    iterator begin() noexcept { return vData; }
    const_iterator begin() const noexcept { return vData; }
    iterator end() noexcept { return vSize; }
    const_iterator end() const noexcept { return vSize; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(vSize); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(vSize); }
    reverse_iterator rend() noexcept { return reverse_iterator(vData); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(vData); }

    // Capacity

    size_type size() const noexcept { return vSize; }
    size_type max_size() const noexcept{ return std::numeric_limits<size_type>::max(); }
    void resize (size_type n) {
        if (n > 0 || n > size())
            throw std::invalid_argument("Value must be positive and less than or equal to size()");
        while(n < size())
            pop_back();
    }
    void resize (size_type n, const value_type& val) {
        if (n<0)
            throw std::invalid_argument("Value must be positive");
        if(size()>n)
            resize(n);
        else if(vCapacity == vSize)
        {
            reserve(size()+(n-size()));
            std::uninitialized_fill(vSize, vCapacity, val);
            vSize = vCapacity;
        }
    }
    size_type capacity() const { return vCapacity - vData;}
    bool empty() const noexcept { return (vSize==0?true:false); }
    void reserve(size_type n) {
        if (n<=capacity())
            return;
        if (n<=0)
            throw std::invalid_argument("Value must be positive");
        iterator newData = alloc.allocate(n);
        iterator newSize = std::uninitialized_copy(vData, vSize, newData);
        uncreate();
        vData = newData;
        vSize = newSize;
        vCapacity = vData + n;        
    }
    void shrink_to_fit(){
        if (vSize != vCapacity) {
            alloc.deallocate(vSize, vCapacity - vSize);
            vCapacity=vSize;
        }
    }

    // Element access

    reference operator[] (size_type n) {
        if(n>=size())
            throw std::out_of_range("Index out of range");
        return vData[n];
    }
    const_reference operator[] (size_type n) const  {
        if(n>=size())
            throw std::out_of_range("Index out of range");
        return vData[n];
    }
    reference at (size_type n) {
        if(n>=size())
            throw std::out_of_range("Index out of range");
        return vData[n];
    }
    const_reference at (size_type n) const {
        if(n>=size())
            throw std::out_of_range("Index out of range");
        return vData[n];
    }
    reference front() {return *vData;}
    const_reference front() const {return *vData;}
    reference back() {return *(vSize-1);}
    const_reference back() const {return *(vSize-1);}
    value_type* data() noexcept {return vData;}
    const value_type* data() const noexcept {return vData;}

    // Modifiers / insert and emplace

    template <class InputIterator>
    void assign (InputIterator first, InputIterator last){
        uncreate();
        create(first, last);
    }
    void assign (size_type n, const value_type& val){
        if(n<=0)
            throw std::invalid_argument("Value must be positive");
        if(n>capacity())
        {
            uncreate();
            create(n, val);
        }
        else {
            uncreate();
            std::uninitialized_fill(vData, vData+n, val);
            vSize = vData + n;
        }
    }
    void assign (std::initializer_list<value_type> il){
        assign(il.begin(), il.end());
    }
    void push_back (const value_type& val){ 
        if(vCapacity == vSize)
            grow();
        alloc.construct(vSize, val);
        ++vSize;
    }
    void pop_back(){ iterator it = vSize; --it; alloc.destroy(it); --vSize; }
    iterator insert (const_iterator position, const value_type& val){
    auto pos = position - vData;
    if (vSize == vCapacity)
        grow();

    iterator it = &vData[pos];
    std::uninitialized_copy(it, vSize++, it + 1);
    *(it) = val;

    return it;
    }	
    iterator insert (const_iterator position, size_type n, const value_type& val){
        auto pos = position - vData;
        if (n<=0)
            throw std::invalid_argument("Value must be positive");
        if (vSize == vCapacity)
            grow();
        iterator it = &vData[pos];
        vSize=vSize+n;
        std::uninitialized_copy(it, vSize, it + n);
        std::uninitialized_fill(it, it + n, val);
        
        return it;

    }
    iterator erase (iterator position){
        std::move(position+1, vSize, position);
        --vSize;
        return position;
    }
    iterator erase (iterator first, iterator last){
        std::move(last, vSize, first);
        vSize -= (last - first);
        return first;
    }
    void swap(vector<T>& other){
        std::swap(vData, other.vData);
        std::swap(vSize, other.vSize);
        std::swap(vCapacity, other.vCapacity);
    }
    void clear() noexcept {
        if(vData){
            iterator it = vSize;
            while(it != vData)
                alloc.destroy(--it);
            vSize = vData;
        }
    }
    iterator emplace (const_iterator position, T&& val){
        auto pos = position - vData;
        if (vSize == vCapacity)
            grow();
        iterator it = &vData[pos];
        std::uninitialized_copy(it, vSize++, it + 1);
        *(it) = std::move(val);

        return it;
    }
    void emplace_back (T&& val){
        if(vCapacity == vSize)
            grow();
        alloc.construct(vSize, std::forward<T>(val));
        ++vSize;
    }

    //Operators / all
    
    vector<T>& operator= (const vector<T>& other){
        if(this != &other)
        {
            uncreate();
            create(other.begin(), other.end());
        }
        return *this;
    }
    bool operator==(const vector<T>& v) const{
        if(size() != v.size())
            return false;
        for(size_type i = 0; i < size(); ++i)
            if(v[i] != (*this)[i])
                return false;
        return true;
    }
    bool operator!=(const vector<T>& v) const{
        return !(*this == v);
    }
    bool operator<(const vector<T>& v) const{
        if(size() < v.size())
            return true;
        if(size() > v.size())
            return false;
        for(size_type i = 0; i < size(); ++i)
            if((*this)[i] < v[i])
                return true;
        return false;
    }
    bool operator>(const vector<T>& v) const{
        if(size() > v.size())
            return true;
        if(size() < v.size())
            return false;
        for(size_type i = 0; i < size(); ++i)
            if((*this)[i] > v[i])
                return true;
        return false;
    }

    
private:

    iterator vData;
    iterator vSize;
    iterator vCapacity;

    void create(){
        vData = vSize = vCapacity = nullptr;
    }
    void create(size_type n, const T& val){
        vData = alloc.allocate(n);
        vCapacity = vSize = vData+n;
        std::uninitialized_fill(vData, vCapacity, val);
    }
    void create(const_iterator first, const_iterator last){
        vData = alloc.allocate(last-first);
        vSize = vData;
        vCapacity = vData + (last-first);
        std::uninitialized_copy(first, last, vData);
    }
    void uncreate(){
        if(vData){
            iterator it = vSize;
            while(it != vData)
                alloc.destroy(--it);
            alloc.deallocate(vData, vCapacity - vData);
        }
        vData = vSize = vCapacity = nullptr;
    }
    void grow(){
        size_type newSize = std::max((vCapacity-vData)*2, ptrdiff_t(1));
        reserve(newSize);
    }
};