#pragma once

#include <type_traits>
#include <utility>
#include <new>

/*
 * References
 *    https://llvm.org/doxygen/ErrorOr_8h_source.html
 *    https://github.com/SerenityOS/serenity/blob/master/AK/Error.h
 */

template<typename T, typename ErrorCodeT>
class ErrorOr {
    static constexpr bool isRef = std::is_reference_v<T>;
    using wrap = std::reference_wrapper<std::remove_reference_t<T>>;

public:
    using storage_type = std::conditional_t<isRef, wrap, T>;

private:
    using reference = std::remove_reference_t<T> &;
    using const_reference = const std::remove_reference_t<T> &;
    using pointer = std::remove_reference_t<T> *;
    using const_pointer = const std::remove_reference_t<T> *;

public:
    template <typename... Args>
    ErrorOr(Args&&... args): has_value(true) {
        new (&storage_value) storage_type(std::forward<Args>(args)...);
    }

    ErrorOr(T&& val): has_value(true) {
        new (&storage_value) storage_type(std::forward<T>(val));
    }

    ErrorOr(ErrorCodeT error_code): has_value(false) {
        new (&storage_error_code) ErrorCodeT(error_code);
    }

    ErrorOr(const ErrorOr &other) {
        has_value = other.has_value;

        if (other.has_value) {
            new (&storage_value) storage_type(other.storage_value);
        } else {
            new (&storage_error_code) ErrorCodeT(other.storage_error_code);
        }
    }

    ErrorOr(ErrorOr&& other) {
        has_value = other.has_value;

        if (other.has_value) {
            new (&storage_value) storage_type(std::move(other.storage_value));
        } else {
            new (&storage_error_code) ErrorCodeT(other.storage_error_code);
        }
    }

    ErrorOr& operator=(ErrorOr const& other) {
        if (this == &other) {
            return *this;
        }

        this->~ErrorOr();
        new (this) ErrorOr(other);
        return *this;
    }

    ErrorOr& operator=(ErrorOr&& other) {
        if (this == &other) {
            return *this;
        }

        this->~ErrorOr();
        new (this) ErrorOr(std::move(other));
        return *this;
    }

    ~ErrorOr() {
        if (has_value) {
            storage_value.~storage_type();
        }
    }

    explicit operator bool() const { return has_value; }

    pointer operator ->() {
        return to_pointer(&storage_value);
    }

    const_pointer operator ->() const {
        return to_pointer(&storage_value);
    }

    reference operator *() {
        return storage_value;
    }

    const_reference operator *() const {
        return storage_value;
    }

    ErrorCodeT get_code() const {
        return storage_error_code;
    }

private:

    // convert reference wrapper to pointer if necessary, otherwise identity
    pointer to_pointer(pointer val) {
        return val;
    }

    const_pointer to_pointer(pointer val) const {
        return val;
    }

    pointer to_pointer(wrap* val) {
        return &val->get();
    }

    const_pointer to_pointer(wrap* val) const {
        return &val->get();
    }

    union {
        ErrorCodeT storage_error_code;
        storage_type storage_value;
    };
    bool has_value;

    template<typename S, typename ErrorCodeS>
    friend bool operator==(ErrorOr<S, ErrorCodeS> const& lhs, ErrorOr<S, ErrorCodeS> const& rhs);
};

template<typename T, typename ErrorCodeT>
bool operator==(ErrorOr<T, ErrorCodeT> const& lhs, ErrorOr<T, ErrorCodeT> const& rhs) {
    if (lhs.has_value && rhs.has_value) {
        return *lhs == *rhs;
    } else if (!lhs.has_value && !rhs.has_value) {
        return lhs.get_code() == rhs.get_code();
    } else {
        return false;
    }
}
