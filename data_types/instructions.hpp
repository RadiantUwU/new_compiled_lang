#pragma once
#include <vector>
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
    ppEndExpr
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
    String,
    Char,
    Token,
    Null,
    True,
    False,
    This,
    Super,
    Float
};
class Instruction {
public:
    Instruction_t type;
    std::vector<void*> args;
    ~Instruction() {
        for (auto& arg : args) {
            delete arg;
        }
    }
};