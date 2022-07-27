#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <sstream>

#include "instructions.hpp"
#include "exception.hpp"

class AST {
    std::vector<AST> children;
    AST* parent = nullptr;
    Instruction_t ty;
    std::vector<std::string> attrs;
    std::string token;
    std::string str_repr(std::string s) {
        std::string sc = "\"";
        for (char c : s) {
            if (c == '\\') {
                sc += "\\\\";
            } else if (c == '"') {
                sc += "\\\"";
            } else {
                sc += c;
            }
        }
        sc += "\"";
        return sc;
    }
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
    std::tuple<std::string, std::string> repr_self() {
        std::stringstream ss;
        ss << (unsigned short)ty;
        std::string beg = "AST(token=" + str_repr(token) + ",children=[";
        std::string end = "],attrs=[";
        for (auto s : attrs) end += str_repr(s);
        end += "],ty=" + ss.str() + ")";
        return std::make_tuple(beg,end);
    }
    std::string repr() {
        std::tuple<std::string, std::string> repr_self_out = repr_self();
        std::string str = std::get<0>(repr_self_out);
        for (auto& a : children) {
            str += a.repr() + ",";
        }
        if (children.size() > 0) str.pop_back();
        str += std::get<1>(repr_self_out);
        return str;
    }
    AST() = default;
};