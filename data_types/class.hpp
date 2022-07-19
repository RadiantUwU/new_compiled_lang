#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "exception.hpp"

class Function_;
class Class_;
class Method_;
using Class = std::shared_ptr<Class_>;
using Function = std::shared_ptr<Function_>;
using Method = std::shared_ptr<Method_>;
using FunctionMap = std::unordered_map<std::string, std::vector<Function>>;
using MethodMap = std::unordered_map<std::string, std::vector<Method>>;
using ClassMap = std::unordered_map<std::string, Class>;
class Class_ {
private:
    void default_init();
protected:
    Class ptrcls;
    friend class Pointer_;
public:
    Class_() {
        default_init();
    };
    Class_(const Class_&) = delete;
    Class_& operator=(const Class_&) = delete;
    std::string name;
    std::vector<Class> bases;
    std::vector<Class> MRO;
    size_t mem_size = 0;
    MethodMap operators;
    MethodMap methods;
    FunctionMap functions;
    MethodMap virtualMethods;
    ClassMap fields;
    Method parent_initializer;
    std::vector<Method> constructors;
    Method destructor;
    Class getptrclass();
    bool isfinal=false;
    bool isabstract=false;
    static void recalcMRO(Class this_) {
        this_->MRO.clear();
        this_->MRO.push_back(this_);
        for (auto& base : this_->bases) {
            for (auto& mro : base->MRO) {
                if (this_->MRO.end() == std::find(this_->MRO.begin(), this_->MRO.end(), mro)) {
                    this_->MRO.push_back(mro);
                }
            }
        }
    }
};
class Pointer_ : public Class_ {
    void pointer_cls_init();
public:
    Pointer_(Class_& cls) {
        cls.ptrcls = std::make_shared<Class_>(this);
        this->name = cls.name + "p";
        pointer_cls_init();
        this->isfinal = true;
    }

};
class Function_ : public Class_ {
    template <typename... Args>
    void f_args(Class c, Args... args) {
        this->args.push_back(c);
        f_args(args...);
    }
    void f_args(Class c) {
        this->args.push_back(c);
    }
    void f_cls_init();
protected:
    Class ret_ty;
    std::vector<Class> args;
public:
    template <typename... Args>
    Function_(Class ret_ty, Args... args) {
        // make sure all args are classes
        static_assert(std::is_base_of<Class_, Args...>::value, "All arguments must be classes");
        this->ret_ty = ret_ty;
        this->f_args(args...);
        this->name = ret_ty->name + " (*)(";
        for (auto& arg : this->args) {
            this->name += arg->name + ", ";
        }
        this->name.pop_back();
        this->name.pop_back();
        this->name += ")";
        this->isfinal = true;
        f_cls_init();
    }
    bool inline_ = false;
};
class Array_ : public Pointer_ {
    void array_cls_init();
public:
    Array_(Class_& cls, size_t size) : Pointer_(cls) {
        this->name = cls.name + "[" + std::to_string(size) + "]";
        this->size = size;
        this->isfinal = true;
        array_cls_init();
    }
    size_t size;
};