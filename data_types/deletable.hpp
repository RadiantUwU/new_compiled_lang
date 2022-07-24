#pragma once

class Deletable {
    void* ptr;
    void (*delete_f)(void*);
public:
    Deletable(void* ptr, void (*delete_f)(void*)) {
        this->ptr = ptr;
        this->delete_f = delete_f;
    }
    ~Deletable() {
        delete_f(ptr); //depends on delete_f to run deallocation
    }
};