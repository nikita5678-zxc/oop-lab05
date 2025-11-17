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

        iterator operator++() {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }
        
        bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const iterator& other) const {
            return *this != other;
        }
    
    private:
        T* ptr_;
    };

    class const_iterator {
    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator() : ptr_(nullptr) {}
        explicit const_iterator(const T* p) : ptr_(p) {}
        const_iterator(const iterator& it) : ptr_(it.ptr_) {}

        reference operator*() {return *ptr_;}
        pointer operator->() {return ptr_;}

        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }

        const_iterator operator++() {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const const_iterator& other) const {
            return *this != other;
        }
    
    private:
        const T* ptr_;
    };
};