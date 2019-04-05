#pragma once

#include <cstddef>
#include <memory>


template<class T>
class ArrayRef {
public:

    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type const* const_pointer;
    typedef value_type& reference;
    typedef value_type const& const_reference;
    typedef value_type* iterator;
    typedef value_type const* const_iterator;

    //
    // constructors
    //

    constexpr ArrayRef() noexcept
        : length(0), data(nullptr)
    { }

    constexpr ArrayRef(ArrayRef const&) noexcept = default;
    constexpr ArrayRef(ArrayRef &&) noexcept = default;

    template<size_t N>
    ArrayRef(T (& a)[N]) noexcept
        : length(N), data(std::addressof(a[0]))  //data(N > 0 ? std::addressof(a[0]) : nullptr)
    {
        static_assert(N > 0, "Zero-length array is not permitted in ISO C++.");
    }

    ArrayRef(pointer p, size_t len) noexcept
        : length(len), data(p)
    {
    }

    // TODO from std::array

    //
    // methods and operators
    //

    ArrayRef& operator=(ArrayRef const&) noexcept = default;
    ArrayRef& operator=(ArrayRef&&) noexcept = default;

    constexpr const_reference operator[](size_t const n) const noexcept {
        // TODO bounds check
        return *(data + n);
    }

    reference operator[](size_t const n) noexcept {
        // TODO bounds check
        return *(data + n);
    }

    constexpr size_t size() const noexcept {
        return length;
    }

    constexpr size_t size_bytes() const noexcept {
        return length * sizeof(T);
    }

    constexpr bool is_empty() const noexcept {
        return length == 0 || data == nullptr;
    }

    //
    // iterators
    //

    constexpr iterator begin() const noexcept {
        return data;
    }

    constexpr iterator end() const noexcept {
        return data + length;
    }

    constexpr const_iterator cbegin() const noexcept {
        return data;
    }

    constexpr const_iterator cend() const noexcept {
        return data + length;
    }

private:
    size_t length;
    pointer data;
};
