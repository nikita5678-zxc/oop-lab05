#pragma once

#include <memory_resource>
#include <cstddef>
#include <stdexcept>

template <typename T>
class Pmr_stack {
public:
    class iterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;
        
        iterator() : ptr_(nullptr) {}
        explicit iterator(T * p) : ptr_(p) {}
        
        reference operator*() {return *ptr_;}
        pointer operator->() {return ptr_;}

        iterator& operator++() {
            ++ptr_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);;
        }
    
    private:
        T* ptr_;
    };

    class const_iterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator() : ptr_(nullptr) {}
        explicit const_iterator(const T* p) : ptr_(p) {}
        const_iterator(const iterator& it) : ptr_(it.ptr_) {}

        reference operator*() const {return *ptr_;}
        pointer operator->() const {return ptr_;}

        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }

        const_iterator operator++(int) {
        const_iterator tmp = *this;
        ++ptr_;
        return tmp;
}

        bool operator==(const const_iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);;
        }
    
    private:
        const T* ptr_;
    };

    explicit Pmr_stack(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr), data_(nullptr), size_(0), capacity_(0) {
        if (mr) {
            capacity_ = 8;
            data_ = alloc_.allocate(capacity_);
        }
    }

    ~Pmr_stack() {
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(&data_[i]);
        }
        if (data_) {
            alloc_.deallocate(data_, capacity_);
        }
    }

    void push(const T& value) {
        if (size_ >= capacity_) {
            grow();
        }
        alloc_.construct(&data_[size_], value);
        ++size_;
    }

    void push(T&& value) {
        if (size_ >= capacity_) {
            grow();
        }
        alloc_.construct(&data_[size_], std::move(value));
        ++size_;
    }
    
    void pop() {
        if (empty()) {
            throw std::underflow_error("stack empty");
        }
        --size_;
        alloc_.destroy(&data_[size_]);
    }

    T& top() {
        if (empty()) {
            throw std::underflow_error("stack empty");
        }
        return data_[size_ - 1];
    }
    
    const T& top() const {
        if (empty()) {
            throw std::underflow_error("stack empty");
        }
        return data_[size_ - 1];
    }

    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}

    iterator begin() noexcept {return iterator(data_);}
    iterator end() noexcept {return iterator(data_ + size_);}

    const_iterator begin() const noexcept {return const_iterator(data_);}
    const_iterator end() const noexcept {return const_iterator(data_ + size_);}
    const_iterator cbegin() const noexcept {return begin();}
    const_iterator cend() const noexcept {return end();}
private:
    std::pmr::polymorphic_allocator<T> alloc_;
    T* data_;
    size_t size_;
    size_t capacity_;

    void grow() {
        size_t new_capacity = capacity_ == 0 ? 8 : capacity_ * 2;
        T* new_data = alloc_.allocate(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            alloc_.construct(&new_data[i], std::move_if_noexcept(data_[i]));
            alloc_.destroy(&data_[i]);
        }

        alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
};