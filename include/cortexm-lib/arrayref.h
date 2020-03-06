#pragma once

#include <cstddef>
#include <memory>


template<typename T>
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
        : length(0),
          data(nullptr)
    { }

    constexpr ArrayRef(ArrayRef const&) noexcept = default;
    constexpr ArrayRef(ArrayRef &&) noexcept = default;

    // from any array-like / vector-like object, including
    // std::array, std::vector, etl::vector, etc.
    template<typename C>
    constexpr ArrayRef(C& c)
        : length(std::end(c) - std::begin(c)),
          data(std::begin(c))
    { }

    // from pair of iterators
    constexpr ArrayRef(pointer start, pointer end)
        : length(end - start),
          data(start)
    { }

    // from C array
    template<size_t N>
    ArrayRef(T (& a)[N]) noexcept
        : length(N),
          data(std::addressof(a[0]))
    {
        static_assert(N > 0, "Zero-length array is not permitted in ISO C++.");
    }

    // with explicit length
    ArrayRef(pointer p, size_t len) noexcept
        : length(len), data(p)
    { }

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
        return data == nullptr ? 0 : length;
    }

    constexpr size_t size_bytes() const noexcept {
        return size() * sizeof(T);
    }

    constexpr bool is_empty() const noexcept {
        return length == 0 || data == nullptr;
    }

    constexpr bool empty() const noexcept {
        return is_empty();
    }

    //
    // iterators
    //

    constexpr iterator begin() noexcept {
        return data;
    }

    constexpr const_iterator begin() const noexcept {
        return data;
    }

    constexpr iterator end() noexcept {
        return data + length;
    }

    constexpr const_iterator end() const noexcept {
        return data + length;
    }

    constexpr const_iterator cbegin() const noexcept {
        return data;
    }

    constexpr const_iterator cend() const noexcept {
        return data + length;
    }

    //
    // slices
    //

    constexpr ArrayRef<T> slice(size_t const pos, size_t const len) {
        // TODO bounds check
        //static_assert(pos + len < length);
        return ArrayRef<T>{begin() + pos, len};
    }

    constexpr ArrayRef<T> slice(size_t const pos, size_t const len) const {
        // TODO bounds check
        //static_assert(pos + len < length);
        return ArrayRef<T>{begin() + pos, len};
    }

private:
    size_t length;
    pointer data;
};
