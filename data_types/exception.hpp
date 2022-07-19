#pragma once
#include <stdexcept>
#include <string>
void* exc_id = 0;
class Exception : public std::runtime_error {
private:
    static void* curr_exc;
public:
    static void* get_exc() {
        return curr_exc;
    }
    static bool error_flag() {
        return curr_exc != nullptr;
    }
    Exception(std::string type,std::string msg) : std::runtime_error(type + (std::string)": " + msg) {
        curr_exc = this;
    }
    static void reset() {
        curr_exc = nullptr;
    }
};
void* Exception::curr_exc = 0;