#pragma once

#include <vector>
#include <string>
#include <algorithm>

#include "instructions.hpp"
#include "exception.hpp"

class AST {
    std::vector<AST> children;
    AST* parent = nullptr;
    Instruction_t ty;
    std::vector<std::string> attrs;
    std::string token;
public:
    AST* getRoot() {
        AST* c = this;
        while (this->parent != nullptr) {
            c = c->parent;
        }
    }
    Instruction_t getType() {
        return ty;
    }
    AST* getParent() {
        return parent;
    }
    const std::vector<AST>& getChildren() {
        return children;
    }
    const std::vector<std::string>& getAttrs() {
        return attrs;
    }
    void pushAttr(std::string a) {
        if (std::find(attrs.begin(),attrs.end(),a) != attrs.end()) return;
        attrs.push_back(a);
    }
    bool operator==(const AST& o) {
        return (size_t)this == (size_t)(void*)&o;
    }
    bool operator!=(const AST& o) {
        return (size_t)this != (size_t)(void*)&o;
    }
    void setType(Instruction_t t) {
        ty = t;
    }
    std::string getToken() {
        return token;
    }
    void setToken(const std::string& s) {
        token = s;
    }
    AST& setParent(AST* p) {
        if (parent != nullptr) throw Exception("InvalidInternalOperation","Tried to set parent to an abstract syntax tree object, but it already had a parent.");
        parent = p;
        parent->children.push_back(*this);
        return parent->children.back();
    }
    void destroy() {
        if (parent != nullptr) parent->children.erase(std::find(parent->children.begin(),parent->children.end(),*this));
    }
    AST() = default;
};