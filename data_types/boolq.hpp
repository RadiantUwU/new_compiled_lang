#pragma once

#include <stdexcept>
#include <type_traits>

#include "exception.hpp"

class boolq final {
public:
    using false_t = std::false_type;
    using true_t = std::true_type;
    using boolq__ = const unsigned char;
    static boolq__ unsure = 2;
private:
    boolq__ val;
public:
    boolq(bool v) : val(v) {}
    boolq(const boolq& o) : val(o.val) {}
    boolq(boolq__ v) : val(v) {
        if (v > 2) throw std::runtime_error("Attempted to give class boolq a non boolq value.");
    }
    bool operator==(const boolq& o) {
        return val == o.val;
    }
    bool operator==(bool o) {
        return val == (boolq__)o;
    }
    bool operator==(const boolq__ v) {
        if (v > 2) throw std::runtime_error("Attempted to give class boolq a non boolq value.");
        return val == v;
    }
    bool operator!=(const boolq& o) {
        return val != o.val;
    }
    bool operator!=(bool o) {
        return val == (boolq__)o;
    }
    bool operator!=(const boolq__ v) {
        if (v > 2) throw std::runtime_error("Attempted to give class boolq a non boolq value.");
        return val != v;
    }
    operator bool() {
        if (val > 1) throw std::runtime_error("Cannot cast to bool: unknown state in boolq");
        return (bool)val;
    }
};