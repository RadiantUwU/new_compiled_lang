#pragma once
#include <vector>

#include "deletable.hpp"

enum class pppi_t : uint8_t {
    String,
    Char,
    Token,
    NewLine
};
enum class ppi_t : uint8_t {
    ppDef,
    ppIf,
    ppElif,
    ppElse,
    ppEndif,
    ppIfndef,
    ppIfdef,
    ppElifndef,
    ppElifdef,
    ppPragma,
    ppInclude,
    ppOpDef,
    ppUndef,
    ppError,
    ppWarning,
    ppdef_name,
    ppdef_arg,
    ppdef_value_start,
    ppdef_value_end,
    ppConcat,
    String,
    Char,
    Token,
    Operator,
    Delimiter,
    groupStart,
    groupEnd,
    NewLine, //do not use extensively, its here for compatiblity
    ppEndExpr,
    Integer,
    Float
};

enum class Instruction_t : uint8_t {
    If,
    Elif,
    Else,
    While_Do,
    Do_While,
    For,
    Break,
    Continue,
    Switch,
    Case,
    New,
    Delete,
    Return,
    Allocate,
    Funcdef,
    Classdef,
    Jump,
    Call,
    Label,
    Operator,
    Copy,
    Integer,
    Float,
    String,
    Char,
    Token,
    Null,
    True,
    False,
    This,
    Class,
    Super,
    CodeObject
};
class Instruction {
public:
    Instruction_t type;
    std::vector<Deletable> args;
    ~Instruction() {
        args.clear();
    }
    void add_inst(void* ptr, void (*d_f)(void*)) {
        args.push_back(Deletable(ptr,d_f));
    };
};