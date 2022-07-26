#pragma once

#include <ctime>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <new>
#include <ostream>
#include <sstream>
#include <initializer_list>
#include <unordered_map>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "data_types/instructions.hpp"
#include "data_types/ast.hpp"
#include "compiler_def_1.hpp"
#include "data_types/exception.hpp"
#include "headers/colors.hpp"
#include "headers/stringUtilities.hpp"
#include "headers/to_string.hpp"
#include "headers/isInVector.hpp"
#define HALF_RAND (((rand() & 0x7FFF) << 1) + (rand() & 0x1))
#define FULL_RAND ((HALF_RAND << 16) + HALF_RAND)
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

class Compiler final {
public:
    class pppi {
    public:
        pppi_t type;
        std::string str;
        pppi(pppi_t type, std::string str) : type(type), str(str) {};
        pppi(pppi_t type) : type(type) {};
        pppi(std::string str) : type(pppi_t::Token), str(str) {};
        operator std::string() const {
            std::stringstream ss;
            ss << "pppi(";
            switch (type) {
                case pppi_t::Char:
                    ss << "Char";
                    break;
                case pppi_t::Token:
                    ss << "Token";
                    break;
                case pppi_t::String:
                    ss << "String";
                    break;
                case pppi_t::NewLine:
                    ss << "NewLine";
                    break;
            }
            ss << ", " << str << ")";
            return ss.str();
        }
    };
    class ppi {
    public:
        ppi_t type;
        std::string str;
        ppi(ppi_t type, std::string str) : type(type), str(str) {};
        ppi(ppi_t type) : type(type) {};
        ppi(std::string str) : type(ppi_t::Token), str(str) {};
        static ppi parseppi(std::string str) {
            if (str == "define") return ppi(ppi_t::ppDef,str);
            else if (str == "if") return ppi(ppi_t::ppIf,str);
            else if (str == "elif") return ppi(ppi_t::ppElif,str);
            else if (str == "else") return ppi(ppi_t::ppElse,str);
            else if (str == "endif") return ppi(ppi_t::ppEndif,str);
            else if (str == "ifndef") return ppi(ppi_t::ppIfndef,str);
            else if (str == "ifdef") return ppi(ppi_t::ppIfdef,str);
            else if (str == "elifndef") return ppi(ppi_t::ppElifndef,str);
            else if (str == "elifdef") return ppi(ppi_t::ppElifdef,str);
            else if (str == "pragma") return ppi(ppi_t::ppPragma,str);
            else if (str == "include") return ppi(ppi_t::ppInclude,str);
            else if (str == "opdef") return ppi(ppi_t::ppOpDef,str);
            else if (str == "undef") return ppi(ppi_t::ppUndef,str);
            else if (str == "error") return ppi(ppi_t::ppError,str);
            else if (str == "warning") return ppi(ppi_t::ppWarning,str);
            throw Exception("UnknownPreProcessorDirective", str);
        };
        static ppi frompppi(pppi p) {
            switch (p.type) {
                case pppi_t::Char:
                    return ppi(ppi_t::Char, p.str);
                case pppi_t::String:
                    return ppi(ppi_t::String, p.str);
                case pppi_t::Token:
                    return ppi(ppi_t::Token, p.str);
                case pppi_t::NewLine:
                    return ppi(ppi_t::NewLine, p.str);
                default:
                    return {0};//null
            }
        }
        ppi removeelse() {
            switch (this->type) {
                case ppi_t::ppElif:
                    return ppi(ppi_t::ppIf, this->str);
                case ppi_t::ppElifdef:
                    return ppi(ppi_t::ppIfdef, this->str);
                case ppi_t::ppElifndef:
                    return ppi(ppi_t::ppIfndef, this->str);
                default:
                    return *this;

            }
        }
        operator std::string() const {
            std::stringstream ss;
            ss << "ppi(";
            switch (type) {
                case ppi_t::Char:
                    ss << "Char";
                    break;
                case ppi_t::String:
                    ss << "String";
                    break;
                case ppi_t::Token:
                    ss << "Token";
                    break;
                case ppi_t::NewLine:
                    ss << "NewLine";
                    break;
                case ppi_t::ppDef:
                    ss << "ppDef";
                    break;
                case ppi_t::ppIf:
                    ss << "ppIf";
                    break;
                case ppi_t::ppElif:
                    ss << "ppElif";
                    break;
                case ppi_t::ppElse:
                    ss << "ppElse";
                    break;
                case ppi_t::ppEndif:
                    ss << "ppEndif";
                    break;
                case ppi_t::ppIfndef:
                    ss << "ppIfndef";
                    break;
                case ppi_t::ppIfdef:
                    ss << "ppIfdef";
                    break;
                case ppi_t::ppElifndef:
                    ss << "ppElifndef";
                    break;
                case ppi_t::ppElifdef:
                    ss << "ppElifdef";
                    break;
                case ppi_t::ppPragma:
                    ss << "ppPragma";
                    break;
                case ppi_t::ppInclude:
                    ss << "ppInclude";
                    break;
                case ppi_t::ppOpDef:
                    ss << "ppOpDef";
                    break;
                case ppi_t::ppUndef:
                    ss << "ppUndef";
                    break;
                case ppi_t::ppError:
                    ss << "ppError";
                    break;
                case ppi_t::ppWarning:
                    ss << "ppWarning";
                    break;
                case ppi_t::ppConcat:
                    ss << "ppConcat";
                    break;
                case ppi_t::ppdef_name:
                    ss << "ppdef_name";
                    break;
                case ppi_t::ppdef_value_start:
                    ss << "ppdef_value_start";
                    break;
                case ppi_t::ppdef_value_end:
                    ss << "ppdef_value_end";
                    break;
                case ppi_t::ppdef_arg:
                    ss << "ppdef_arg";
                    break;
                case ppi_t::Operator:
                    ss << "Operator";
                    break;
                case ppi_t::Delimiter:
                    ss << "Delimiter";
                    break;
                case ppi_t::groupStart:
                    ss << "groupStart";
                    break;
                case ppi_t::groupEnd:
                    ss << "groupEnd";
                    break;
                case ppi_t::ppEndExpr:
                    ss << "ppEndExpr";
                    break;
                case ppi_t::Integer:
                    ss << "Integer";
                    break;
                case ppi_t::Float:
                    ss << "Float";
                    break;

            }
            ss << ", " << str << ")";
            return ss.str();
        }
        bool operator== (const ppi& other) const {
            return this->type == other.type && this->str == other.str;
        }
        bool operator!= (const ppi& other) const {
            return !(*this == other);
        }
        bool operator== (const std::string& other) const {
            return this->str == other;
        }
        bool operator!= (const std::string& other) const {
            return !(*this == other);
        }
    };
    class Definition {
        public:
        std::vector<std::string> args;
        std::vector<ppi> val;
        virtual std::vector<ppi> parse(std::vector<std::vector<ppi>> args_a) {
            std::vector<ppi> ret;
            std::vector<ppi>* args_aap = nullptr;
            if ((args_a.size() != args.size()) && args.back() != "__va_args__")
                throw Exception("DefinitionParsingError", "args_a.size() != args.size(); not enough/too many arguments given to macro call.");
            if (args.back() == "__va_args__") {
                args_aap = new std::vector<ppi>;
                unsigned short optargs = args_a.size() - (args.size() - 1);
                for (unsigned short argp = (args.size() - 1); argp < (args_a.size() + 1);argp++) {
                    for (auto i : args_a[argp]) args_aap->push_back(i);
                    args_aap->push_back(ppi(ppi_t::Delimiter,","));
                }
                if (optargs > 0) args_aap->pop_back();
                for (unsigned short i = 0; i < optargs; i++) args_a.pop_back();
                
            }
            for (ppi& p : val) {
                if (p.type == ppi_t::Token) {
                    if (p.str == "__VA_ARGS__") {
                        if (args_aap != nullptr) {
                            for (auto& p : *args_aap) {
                                ret.push_back(p);
                            }
                        }
                        goto next;
                    }
                    auto findout = std::find(args.begin(),
                                  args.end(),
                                  p.str);
                    if (findout != args.end()) {
                        auto index = std::distance(args.begin(), findout);
                        for (auto& p : args_a[index]) {
                            ret.push_back(p);
                        }
                    } else goto notfound;
                } else {
                    notfound:
                    ret.push_back(p);
                }
                next: ;
            }
            if (args_aap != nullptr) delete args_aap;
            return ret;
        }
        Definition(const std::vector<std::string>& args, const std::vector<ppi>& val) : args(args), val(val) {};
        Definition(const std::initializer_list<std::string>& args, const std::initializer_list<ppi>& val) : args(args), val(val) {};
        Definition() = default;
    };
    std::vector<std::string> includepath;
    bool nobase = false;
    enum class OptimizationLevel {
        None,
        Minimal,
        Full,
        Maximum
    } olevel = OptimizationLevel::None;
private:
    const char* arr = "0123456789abcdef";
    class Logger {
        int64_t currt;
        bool& verbose;
    public:
        Logger(Compiler& c) : verbose(c.verbose) {}
        void begin() {
            currt = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        }
        void debug(const std::string& str) {
            if (verbose) {
                std::cout << COLOR_GREEN "[DEBUG " << (double)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - currt) / 1000 << "] " << str << COLOR_RESET << std::endl;
            }
        }
        void info(const std::string& str) {
            std::cout << COLOR_BLUE "[INFO " << (double)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - currt) / 1000 << "] " << str << COLOR_RESET << std::endl;
        }
        void warn(const std::string& str) {
            std::cout << COLOR_YELLOW "[WARN " << (double)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - currt) / 1000 << "] " << str << COLOR_RESET << std::endl;
        }
        void error(const std::string& str) {
            std::cout << COLOR_RED "[ERROR " << (double)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - currt) / 1000 << "] " << str << COLOR_RESET << std::endl;
        }
        void fatal(const std::string& str) {
            std::cout << COLOR_DARK_RED "[FATAL " << (double)(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - currt) / 1000 << "] " << str << COLOR_RESET << std::endl;
            exit(1);
        }
        
    } logger;
    class ForEveryDefinition : public Definition {
    public:
        virtual std::vector<ppi> parse(std::vector<std::vector<ppi>> args_a) {
            std::vector<ppi> ret;
            if (args_a.size() > 1) {
                if (args_a[0].size() != 1) return ret;
                if (args_a[0][0].type != ppi_t::Token) return ret;
                for (unsigned short i = 1; i < args_a.size(); i++) {
                    ret.push_back(args_a[0][0]);
                    ret.push_back(ppi(ppi_t::groupStart,"("));
                    for (auto& iv : args_a[i]) ret.push_back(iv);
                    ret.push_back(ppi(ppi_t::groupEnd,")"));
                }
            }
            return ret;
        }
    };
    class GetTDefDefinition : public Definition {
        virtual std::vector<ppi> parse(std::vector<std::vector<ppi>> args_a) {
            std::vector<ppi> ret;
            if (args_a.size() > 1) {
                if (args_a[0].size() != 1) return ret;
                if (args_a[0][0].type != ppi_t::Token) return ret;
                unsigned short c = std::stoi(args_a[0][0].str);
                args_a.erase(args_a.begin());
                if (c >= args_a.size()) return ret;
                ret = std::vector<ppi>(args_a[c].begin(),args_a[c].end());
            }
            return ret;
        }
    };
    class HasArgsDefinition : public Definition {
        virtual std::vector<ppi> parse(std::vector<std::vector<ppi>> args_a) {
            std::vector<ppi> ret = {
                ppi(ppi_t::ppIfdef,"ifdef"),
                ppi(ppi_t::Token,"HAS_ARGS_CHK"),
                ppi(ppi_t::ppEndExpr),
                ppi(ppi_t::ppUndef,"undef"),
                ppi(ppi_t::ppdef_name,"HAS_ARGS_CHK"),
                ppi(ppi_t::ppEndif,"endif")
            };
            if (args_a.size() > 0) {
                ret.push_back(ppi(ppi_t::ppDef,"define"));
                ret.push_back(ppi(ppi_t::ppdef_name,"HAS_ARGS_CHK"));
                ret.push_back(ppi(ppi_t::ppdef_value_start));
                ret.push_back(ppi(ppi_t::ppdef_value_end));
            }
            return ret;
        }
    };
    std::vector<std::string> codes;
    std::vector<std::string> includestack;
    std::vector<std::string> defstack;
    struct IncludedAlready_t {
        int64_t hash;
        bool reuseavail = true;
        IncludedAlready_t(int64_t hash) : hash(hash) {}
        IncludedAlready_t() : hash(0) {}
        IncludedAlready_t(const IncludedAlready_t& other) : hash(other.hash), reuseavail(other.reuseavail) {}
        IncludedAlready_t& operator=(const IncludedAlready_t& other) {
            hash = other.hash;
            reuseavail = other.reuseavail;
            return *this;
        }
        bool operator==(const IncludedAlready_t& other) const {
            return hash == other.hash;
        }
        bool operator!=(const IncludedAlready_t& other) const {
            return hash != other.hash;
        }
        bool operator==(int64_t other) const {
            return hash == other;
        }
        bool operator!=(int64_t other) const {
            return hash != other;
        }
    };
    std::vector<IncludedAlready_t> includedalready;
    enum class def_arg_stg : uint8_t {
        waiting_start,
        next_arg
    };
    std::unordered_map<std::string, Definition> definitions;
    std::vector<ppi> def_args;
    std::vector<pppi> uncommentedcode;
    std::vector<ppi> code_interPPI;
    std::vector<ppi> code_runPPI;
    std::vector<ppi> code_afterPPI;
    std::vector<ppi> code_numliteral;
    AST code_stg6;
    AST& current = code_stg6;
    friend class blr_;
    void ast_back() {
        if (current.getParent() == nullptr) throw Exception("InvalidInternalOperation", "Attempted to do an AST back operation on root of tree.");
        current = *(current.getParent());
    }
    void ast_forward() {
        if (current.getChildren().size() == 0) throw Exception("InvalidInternalOperation", "Attempted to do an AST forward operation on an object without children");
        current = current.getChildren().back();
    }
    void ast_forward_new() {
        current = AST().setParent(&current);
    }
    void ast_setInst(Instruction_t t) {
        current.setType(t);
    }
    void ast_setCurrent(AST& a) {
        current = a;
    }
    void ast_addAttr(std::string a) {
        current.pushAttr(a);
    }
    void ast_setToken(std::string s) {
        current.setToken(s);
    }
    
    void ast_new__(Instruction_t t, std::string s) {
        ast_forward_new();
        ast_setInst(t);
        ast_setToken(s);
    }
    void ast_new___(Instruction_t t) {
        ast_forward_new();
        ast_setInst(t);
    }
    
    void ast_newString(std::string s) {
        ast_new__(Instruction_t::String,s);
    }
    void ast_newChar(std::string s) {
        ast_new__(Instruction_t::Char,s);
    }
    void ast_newToken(std::string s) {
        ast_new__(Instruction_t::String,s);
    }
    void ast_newOperator(std::string s) {
        ast_new__(Instruction_t::Operator,s);
    }
    void ast_newInteger(std::string s) {
        ast_new__(Instruction_t::Integer,s);
    }
    void ast_newFloat(std::string s) {
        ast_new__(Instruction_t::Float,s);
    }
    void ast_newClass(std::string s) {
        ast_new__(Instruction_t::Class,s);
    }
    void ast_newThis() {
        ast_new___(Instruction_t::This);
    }
    class Operators {
    public:
        struct op {
            std::string opr;
            signed short order;
            std::vector<std::string> attr;
            op(std::string o, signed short v) : opr(o), order(v) {}
            op() = default;
            bool isbraces() {
                if (opr.size() == 2) {
                    if (opr == "()") return true;
                    if (opr == "[]") return true;
                    if (opr == "{}") return true;
                }
                return false;
            }
        };
        std::vector<op> ops;
        void push_back(const std::string& s) {
            ops.push_back(op(s,0));
        }
        void push_back(const std::string& s, signed short v) {
            ops.push_back(op(s,v));
        }
        void push_back(const std::string& s, signed short v, const std::vector<std::string>& a) {
            ops.push_back(op(s, v));
            op& c = ops.back();
            for (auto& i : a) {
                c.attr.push_back(i);
            }
        }
        auto front() {
            return ops.front();
        }
        auto back() {
            return ops.back();
        }
        auto size() {
            return ops.size();
        }
        auto begin() {
            return ops.begin();
        }
        auto end() {
            return ops.end();
        } //allow looping
        auto findbyattr(const std::string& c) {
            for (auto& i : ops) {
                if (i.attr.begin() != i.attr.end())
                if (std::find(i.attr.begin(),i.attr.end(),c) != i.attr.end())
                return i;
            }
            return op();
        }
    } def_ops;
    using op = Operators::op;
    struct temp_t {
        std::string s;
        unsigned long long l;
        bool isstring = false;
        temp_t(const std::string& s) : s(s), isstring(true) {}
        temp_t(unsigned long long l) : l(l) {}
        temp_t() = default;
    } temp[4];
    template <typename T>
    static std::string str_code(const std::vector<T>& v) {
        std::stringstream ss;
        for (auto& i : v) {
            ss << (std::string)i;
        }
        return ss.str();
    }
    struct IncludeFileOutput {
        std::string code;
        enum class MsgType : uint8_t {
            OK,
            NotFound,
            AlreadyIncluded,
            CyclicInclude
        } msg;
        uint64_t hash = 0;
    };
    IncludeFileOutput include1(const std::string& filename) {
        IncludeFileOutput ret;
        if (includestack.size() > 0) {
            for (auto& i : includestack) {
                if (i == filename) {
                    ret.msg = IncludeFileOutput::MsgType::CyclicInclude;
                    return ret;
                }
            }
        }
        std::ifstream file;
        bool err = 1;
        for (auto& path : includepath) {
            std::string f = path + "/" + filename;
            file.open(f);
            if (file.good()) {
                err = 0; // no error
                break;
            }
        }
        if (err) {
            ret.msg = IncludeFileOutput::MsgType::NotFound;
            return ret;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        ret.code = ss.str();
        uint64_t h = std::hash<std::string>()(ret.code);
        ret.hash = h;
        ret.msg = (std::find(includedalready.begin(), includedalready.end(), h) != includedalready.end()) ? IncludeFileOutput::MsgType::AlreadyIncluded : IncludeFileOutput::MsgType::OK;
        return ret;
    }
    void includefile(const std::string& filename) {
        IncludeFileOutput o = include1(filename);
        switch (o.msg) {
            case IncludeFileOutput::MsgType::OK:
                goto includeanyway;
            case IncludeFileOutput::MsgType::AlreadyIncluded:
                if (std::find(includedalready.begin(), includedalready.end(), o.hash)->reuseavail) {
                    goto includeanyway;
                } else {
                    logger.debug("Already included " + filename);
                }
                return;
            case IncludeFileOutput::MsgType::CyclicInclude:
                logger.fatal("Cyclic include " + filename);
                throw Exception("CyclicInclude", "Cyclic include " + filename);
            case IncludeFileOutput::MsgType::NotFound:
                logger.fatal("Include file not found " + filename);
                throw Exception("NotFoundInclude", "Include file not found " + filename);
            default:
                logger.fatal("Unknown case during include, memory corruption?");
                abort();
        }
        includeanyway:
        includestack.push_back(filename);
        logger.debug("Begin compilation of " + filename);
        build_stage_1(o.code);
        build_stage_2();
        build_stage_3();
        build_stage_4();
        includestack.pop_back();
        logger.debug("End compilation of " + filename);
        return;
    }
public:
    void addDef(const std::string& name, const std::vector<std::string>& args, const std::vector<ppi>& val) {
        definitions[name] = Definition(args, val);
    }
    void addDef(const std::string& name, const std::initializer_list<std::string>& args, const std::initializer_list<ppi>& val) {
        definitions[name] = Definition(args, val);
    }
    void addOp(const std::string& oper) {
        def_ops.push_back(oper);
    }
    void addOp(const std::string& oper, const unsigned short v) {
        def_ops.push_back(oper,v);
    }
    void addOp(const std::string& oper, const unsigned short v, std::vector<std::string> attr) {
        def_ops.push_back(oper,v,attr);
    }
    void build_stage_1(std::string code) {
        logger.debug("Uncommenting code and doing first token split...");
        std::string buffer;
        std::string buffer_esc;
        unsigned char t = 0;
        enum class bl {
            str,
            str_esc,
            chr,
            chr_esc,
            none,
            comment,
            block_comment,
            str_esc_bin,
            str_esc_oct,
            str_esc_hex,
            chr_esc_bin,
            chr_esc_oct,
            chr_esc_hex
        } building = bl::none;
        code += '\n';
        for (auto ch : code) {
            switch (building) {
            case bl::str:
            case bl::chr:
                if (ch == '\\') 
                    building = (building == bl::str) ? bl::str_esc : bl::chr_esc;
                else if (ch == '"' && building == bl::str) 
                    goto token_break;
                else if (ch == '\'' && building == bl::chr) 
                    goto token_break;
                else if (ch == '\n') 
                    throw Exception("UnendedQuote","Unended quote for char/string literal.");
                else 
                    buffer += ch;
                break;
            case bl::str_esc:
            case bl::chr_esc:
                switch (ch) {
                case 'n':
                    buffer += '\n';
                    building = (building == bl::str_esc) ? bl::str : bl::chr;
                    break;
                case 't':
                    buffer += '\t';
                    building = (building == bl::str_esc) ? bl::str : bl::chr;
                    break;
                case 'r':
                    buffer += '\r';
                    building = (building == bl::str_esc) ? bl::str : bl::chr;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                    buffer_esc += ch;
                    t = 1;
                    building = (building == bl::str_esc) ? bl::str_esc_oct : bl::chr_esc_oct;
                    break;
                case 'x':
                    t = 0;
                    building = (building == bl::str_esc) ? bl::str_esc_hex : bl::chr_esc_hex;
                    break;
                case 'b':
                    t = 0;
                    building = (building == bl::str_esc) ? bl::str_esc_bin : bl::chr_esc_bin;
                    break;
                default:
                    buffer += ch;
                    building = (building == bl::str_esc) ? bl::str : bl::chr;
                    break;
                }
                break;
            case bl::str_esc_bin:
            case bl::chr_esc_bin:
                if (ch == '0' || ch == '1') {
                    buffer_esc += ch;
                    t++;
                    if (t == 8) {
                        buffer += (char)std::stoi(buffer_esc, nullptr, 2);
                        t = 0;
                        buffer_esc.clear();
                        building = (building == bl::str_esc_bin) ? bl::str : bl::chr;
                    }
                } else {
                    throw Exception("InvalidBinEscape","Invalid binary escape.");
                }
                break;
            case bl::str_esc_oct:
            case bl::chr_esc_oct:
                if (ch >= '0' && ch <= '7') {
                    buffer_esc += ch;
                    t++;
                    if (t == 3) {
                        buffer += (char)std::stoi(buffer_esc, nullptr, 8);
                        t = 0;
                        buffer_esc.clear();
                        building = (building == bl::str_esc_oct) ? bl::str : bl::chr;
                    }
                } else {
                    throw Exception("InvalidOctEscape","Invalid octal escape.");
                }
                break;
            case bl::str_esc_hex:
            case bl::chr_esc_hex:
                if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
                    buffer_esc += ch;
                    t++;
                    if (t == 2) {
                        buffer += (char)std::stoi(buffer_esc, nullptr, 16);
                        t = 0;
                        buffer_esc.clear();
                        building = (building == bl::str_esc_hex) ? bl::str : bl::chr;
                    }
                } else {
                    throw Exception("InvalidHexEscape","Invalid hexadecimal escape.");
                }
                break;
            default:
                switch (ch) {
                case '\r':
                    break;
                case ' ':
                case '\t':
                case '\n':
                    token_break:
                    if (buffer.size() > 0) {
                        pppi p(buffer);
                        switch (building) {
                            case bl::str:
                                p.type = pppi_t::String;
                                p.str += ch;
                                break;
                            case bl::chr:
                                p.type = pppi_t::Char;
                                p.str += ch;
                                break;
                            default:
                                break;
                        }
                        uncommentedcode.push_back(p);
                        buffer.clear();
                        if (ch == '\n') uncommentedcode.push_back(pppi(pppi_t::NewLine));
                    }
                    break;
                case '/':
                    if (building == bl::none) {
                        if (buffer.back() == '/') {
                            building = bl::comment;
                            buffer.pop_back();
                            if (buffer.size() > 0) {
                                uncommentedcode.push_back(pppi(buffer));
                                buffer.clear();
                            }
                        } else if (buffer.back() == '*') {
                            building = bl::block_comment;
                            buffer.pop_back();
                            if (buffer.size() > 0) {
                                uncommentedcode.push_back(pppi(buffer));
                                buffer.clear();
                            }
                        } else {
                            buffer += ch;
                        }
                        
                    } else if (building == bl::block_comment) {
                        if (buffer.back() == '*') {
                            building = bl::none;
                            buffer.clear();
                        } else {
                            buffer += ch;
                        }
                    } else {
                        buffer += ch;
                    }
                    break;
                case '"':
                    if (building == bl::none) {
                        building = bl::str;
                    } else if (building == bl::str) {
                        building = bl::none;
                        uncommentedcode.push_back(pppi(pppi_t::String,buffer));
                        buffer.clear();
                    } else if (building == bl::str_esc) {
                        building = bl::str;
                        buffer += ch;
                    } else {
                        buffer += ch;
                    }
                    break;
                case '\'':
                    if (building == bl::none) {
                        building = bl::chr;
                    } else if (building == bl::chr) {
                        building = bl::none;
                        uncommentedcode.push_back(pppi(pppi_t::Char,buffer));
                        buffer.clear();
                    } else if (building == bl::chr_esc) {
                        building = bl::chr;
                        buffer += ch;
                    } else {
                        buffer += ch;
                    }
                    break;
                default:
                    buffer += ch;
                    break;
            }
            
            }
            next:
            (void)0;
        }
        logger.debug("Uncommenting code and doing first token split... done");
    }
    void build_stage_2() {
        logger.debug("Splitting further...");
        std::string buffer;
        std::vector<pppi> oldcode = uncommentedcode;
        uncommentedcode.clear();
        enum class bl {
            none,
            ppi_,
            def_block,
            def_block_nl,
            def_name,
            def_args,
            undef_name,
            if_expr,
            
        } building = bl::none;
        enum class bl2 : uint8_t {
            none,
            oper,
            tok,
        } bbl2 = bl2::none;
        #define flush_build2_buffer_compiler__comp_bl2_stg if (bbl2 != bl2::none) {\
                                ppi_t typep = (ppi_t)0;\
                                switch (bbl2) {\
                                    case bl2::oper:\
                                        typep = ppi_t::Operator;\
                                        break;\
                                    case bl2::tok:\
                                        typep = ppi_t::Token;\
                                        break;\
                                    default:\
                                        break;\
                                }\
                                code_interPPI.push_back(ppi(typep,buffer));\
                                buffer.clear();\
                                bbl2 = bl2::none;\
                            }
        unsigned long char_index = 0;
        for (auto& p : oldcode) {
            statementReset:
            if (char_index >= p.str.size() && p.type != pppi_t::NewLine) goto nextStatement;
            switch (building) {
            case bl::none:
                switch (p.type) {
                    case pppi_t::NewLine:
                        flush_build2_buffer_compiler__comp_bl2_stg
                        goto nextStatement;
                    case pppi_t::Char:
                        flush_build2_buffer_compiler__comp_bl2_stg
                        code_interPPI.push_back(ppi(ppi_t::Char,p.str));
                        goto nextStatement;
                    case pppi_t::String:
                        flush_build2_buffer_compiler__comp_bl2_stg
                        code_interPPI.push_back(ppi(ppi_t::String,p.str));
                        goto nextStatement;
                    default: break;
                }
                while (char_index < p.str.size()) {
                    evalch:
                    char ch = *(p.str.begin() + char_index);
                    switch (ch) {
                        case '#':
                            if (buffer.empty()) {
                                building = bl::ppi_;
                                char_index++;
                                goto statementReset;
                            }
                        case '+':
                        case '-':
                        case '*':
                        case '/':
                        case '%':
                        case '^':
                        case '&':
                        case '|':
                        case '<':
                        case '>':
                        case '=':
                        case '!':
                        case '~':
                        case '?':
                        case ':':
                        case '$':
                        case '`':
                        case '@':
                        case '.':
                            if (bbl2 == bl2::tok) {
                                bbl2 = bl2::oper;
                                code_interPPI.push_back(ppi(ppi_t::Token,buffer));
                                buffer.clear();
                                buffer.push_back(ch);
                            }
                            break;
                        case ',':
                        case ';':
                            flush_build2_buffer_compiler__comp_bl2_stg
                            buffer.push_back(ch);
                            code_interPPI.push_back(ppi(ppi_t::Delimiter,buffer));
                            buffer.clear();
                            break;
                        case '\\':
                            throw Exception("UnexpectedCharacter","Unexpected escape character.");
                        case '(':
                        case '[':
                        case '{':
                            flush_build2_buffer_compiler__comp_bl2_stg
                            buffer.push_back(ch);
                            code_interPPI.push_back(ppi(ppi_t::groupStart,buffer));
                            buffer.clear();
                            break;
                        case ')':
                        case ']':
                        case '}':
                            flush_build2_buffer_compiler__comp_bl2_stg
                            buffer.push_back(ch);
                            code_interPPI.push_back(ppi(ppi_t::groupEnd,buffer));
                            buffer.clear();
                            break;
                        default:
                            if (bbl2 == bl2::oper) {
                                bbl2 = bl2::tok;
                                code_interPPI.push_back(ppi(ppi_t::Operator,buffer));
                                buffer.clear();
                            }
                            buffer.push_back(ch);
                            break;
                    }
                    char_index++;
                }
                break;
            case bl::ppi_:
                if (true){
                if (p.type == pppi_t::NewLine) 
                    throw Exception("UnexpectedNewLineInPreProcessor","");
                else if (p.type == pppi_t::Char)
                    throw Exception("UnexpectedCharInPreProcessor","");
                else if (p.type == pppi_t::String)
                    throw Exception("UnexpectedStringInPreProcessor","");
                else if (*(p.str.begin() + char_index) == '#') {
                    building = bl::none;
                    char_index++;
                    code_interPPI.push_back(ppi(ppi_t::ppConcat,"##"));
                    goto statementReset;
                } else {
                    ppi np = ppi::parseppi(p.str.substr(char_index));
                    code_interPPI.push_back(np);
                    switch (np.type) {
                        case ppi_t::ppDef:
                            building = bl::def_name;
                            goto nextStatement;
                        case ppi_t::ppUndef:
                            building = bl::undef_name;
                            goto nextStatement;
                        case ppi_t::ppIf:
                        case ppi_t::ppIfdef:
                        case ppi_t::ppIfndef:
                        case ppi_t::ppElif:
                        case ppi_t::ppElifdef:
                        case ppi_t::ppElifndef:
                        case ppi_t::ppInclude:
                        case ppi_t::ppPragma:
                        case ppi_t::ppError:
                        case ppi_t::ppWarning:
                        case ppi_t::ppOpDef:
                            building = bl::if_expr;
                            goto nextStatement;
                        default:
                            building = bl::none;
                            goto nextStatement;
                    }
                }
                break;}
            case bl::def_name:
                if (p.type != pppi_t::Token)
                    throw Exception("UnexpectedCharacter", "Unexpected character after #define preprocessor.");
                if (std::find(p.str.begin(),p.str.end(),'(') != p.str.end()) { //has arguments
                    auto pos = p.str.find('(');
                    // split string by '(' and use first string to push definition name
                    code_interPPI.push_back(ppi(ppi_t::ppdef_name,p.str.substr(0,pos)));
                    // good AI
                    // there may be spaces between the arguments, make sure you handle first after '(' then go handle automatically
                    // first check if the string has ')'
                    if (p.str.find(')') != std::string::npos) {
                        // get the substringed
                        auto args = p.str.substr(pos+1,p.str.find(')')-pos-1);
                        // split by ',' and push args
                        for (auto arg : split(args,',')) {
                            code_interPPI.push_back(ppi(ppi_t::ppdef_arg,arg));
                        }
                        // define name finished, go to block
                        building = bl::def_block;
                        goto nextStatement;
                    } else {
                        // if it does not have ')' then get the first argument and push it then set building to def_args
                        code_interPPI.push_back(ppi(ppi_t::ppdef_arg,p.str.substr(pos+1)));
                        building = bl::def_args;
                        goto nextStatement;
                    }
                }
                else {
                    code_interPPI.push_back(ppi(ppi_t::ppdef_name,p.str));
                    building = bl::def_block;
                    goto nextStatement;
                }
            case bl::def_args:
                // must be token or throw error
                if (p.type != pppi_t::Token)
                    throw Exception("UnexpectedCharacter", "Unexpected character after definition arguments start token.");
                // enter scope to allow variables
                if (true) {
                    auto str = p.str;
                    bool terminate = false;
                    // remove ',' from string if it contains it at the beginning
                    if (str.front() == ',')
                        str.erase(str.begin());
                    // check if string contains ')'
                    if (str.find(')') != std::string::npos) {
                        // if it does, remove it and set terminate to true
                        str.erase(str.find(')'));
                        terminate = true;
                    }
                    // check if string contains ',' then split string by ',' and push args
                    if (str.find(',') != std::string::npos) {
                        for (auto arg : split(str,',')) {
                            code_interPPI.push_back(ppi(ppi_t::ppdef_arg,arg));
                        }
                    } else {
                        // if it does not, push the string
                        code_interPPI.push_back(ppi(ppi_t::ppdef_arg,str));
                    }
                    // if terminate is true, enter def_block
                    if (terminate) {
                        building = bl::def_block;

                    }
                    goto nextStatement;
                }
            case bl::def_block:
                // check if new line
                if (p.type == pppi_t::NewLine) {
                    // if it is, exit definition
                    building = bl::none;
                    code_interPPI.push_back(ppi(ppi_t::ppdef_value_end));
                    goto nextStatement;
                } else if (p.type == pppi_t::Token) {
                    //check if token is '\\'
                    if (p.str == "\\") {
                        // if it is, go to def_block_nl
                        building = bl::def_block_nl;
                        goto nextStatement;
                    }
                }
                // push token
                code_interPPI.push_back(ppi::frompppi(p));
                goto nextStatement;
            case bl::def_block_nl:
                if (p.type == pppi_t::NewLine) {
                    // if it is, go to def_block
                    building = bl::def_block;
                    goto nextStatement;
                } else 
                    // throw unexpected character error
                    throw Exception("UnexpectedCharacter", "Unexpected character after '\\'.");
            case bl::undef_name:
                if (p.type != pppi_t::Token)
                    throw Exception("UnexpectedCharacter", "Unexpected character after #undef preprocessor.");
                code_interPPI.push_back(ppi(ppi_t::ppdef_name,p.str));
                building = bl::none;
                goto nextStatement;
            case bl::if_expr:
                if (p.type != pppi_t::NewLine) {
                    code_interPPI.push_back(ppi::frompppi(p));
                    goto nextStatement;
                } else {
                    building = bl::none;
                    goto nextStatement;
                }
            }
            nextStatement:
            char_index = 0;
        }
        #undef flush_build2_buffer_compiler__comp_bl2_stg
        logger.debug("Splitting further... done");
    }
    void build_stage_3() {
        logger.debug("Preparing code for preprocessor execution...");
        std::vector<ppi> oldcode = code_interPPI;
        code_interPPI.clear();
        for (auto p : oldcode) switch (p.type) {
            case ppi_t::NewLine:
                break;
            default:
                code_runPPI.push_back(p);
                break;
        }
        logger.debug("Preparing code for preprocessor execution... done");
    }
    void build_stage_4() {
        logger.debug("Running preprocessor...");
        std::vector<ppi> oldcode = code_runPPI;
        code_runPPI.clear();
        
        Definition def;

        std::vector<ppi> ifexpr_parse;
        unsigned short ifexpr_eval = 0;
        unsigned short elseexpr_eval = 0;

        Definition* defadr;
        std::string defname;
        std::vector<std::vector<ppi>> def_args;
        unsigned short defarg_index = 0;
        def_arg_stg def_stg = def_arg_stg::waiting_start;

        ppi_t currinst = ppi_t::NewLine; //null
        // ppi_t::ppElif for when an if evaluates to false
        for (auto i : oldcode) {
            reset:
            if (ifexpr_eval > 0) switch (i.type) {
                case ppi_t::ppElse:
                    if (ifexpr_eval == 1) ifexpr_eval = 0;
                    goto next;
                case ppi_t::ppEndif:
                    ifexpr_eval--;
                    goto next;
                case ppi_t::ppIf:
                case ppi_t::ppIfdef:
                case ppi_t::ppIfndef:
                    ifexpr_eval++;
                    goto next;
                case ppi_t::ppElif:
                case ppi_t::ppElifdef:
                case ppi_t::ppElifndef:
                    if (ifexpr_eval == 1) {
                        ifexpr_eval = 0;
                        i = i.removeelse();
                        goto reset;
                    }
                default:
                    goto next;
            }
            switch (currinst) {
                case ppi_t::NewLine:
                    switch (i.type) {
                        case ppi_t::ppDef: //handled
                        case ppi_t::ppIf: //took, but not handled
                        case ppi_t::ppIfdef: //handled
                        case ppi_t::ppIfndef: //handled
                        case ppi_t::ppInclude: //handled
                        case ppi_t::ppUndef: //handled
                        case ppi_t::ppError: //handled
                        case ppi_t::ppWarning: //handled
                        case ppi_t::ppConcat: //handled
                        case ppi_t::ppOpDef: //handled
                        case ppi_t::ppPragma: //handled
                            currinst = i.type;
                            goto next;
                        case ppi_t::ppElse:
                        case ppi_t::ppElif:
                        case ppi_t::ppElifdef:
                        case ppi_t::ppElifndef:
                            currinst = ppi_t::ppElse;
                        case ppi_t::ppEndif:
                            logger.debug("Endif token");
                            goto next;
                        case ppi_t::ppEndExpr:
                            logger.warn("Unexpected endexpr token.");
                            goto next;
                        case ppi_t::Operator:
                            if (i.str == "%#") {
                                currinst = ppi_t::String;
                                goto next;
                            }
                        default:
                            defcheck:
                            if (true) {
                                auto c = definitions.find(i.str);
                                if (c != definitions.end()) {
                                    if (std::find(
                                        defstack.begin(),
                                        defstack.end(),
                                        c->first
                                    ) != defstack.end()) {
                                        throw Exception("RecursiveDefinitonCall", "Recursive definition call of \"" + c->first + "\".");
                                    }
                                    Definition d = c->second;
                                    if (d.args.size() == 0) {
                                        logger.debug("Begin parsing macro \"" + c->first + "\"");
                                        defstack.push_back(c->first);
                                        for (auto j : d.val) {
                                            code_runPPI.push_back(j);
                                        }
                                        build_stage_4();
                                        defstack.pop_back();
                                        logger.debug("End parsing macro \"" + c->first + "\"");
                                    } else {
                                        currinst = ppi_t::ppdef_arg;
                                        defadr = &(c->second);
                                        def_args.clear();
                                        defname = c->first;
                                    }
                                    goto next;
                                } else if (i.str == "__RAND__") {
                                    uint32_t num = FULL_RAND;
                                    i.str = "__rand_out_";
                                    for (unsigned char j = 0; j < 8; j++) {
                                        i.str+= arr[(num >> (4*(7-j))) % 16];
                                    }
                                    goto reset;
                                } else {
                                    code_afterPPI.push_back(i);
                                    goto next;
                                }
                            }
                            goto next;
                    }
                case ppi_t::ppConcat:
                    if (i.type != code_afterPPI.back().type) throw Exception("InvalidConcatenation","Cannot concat two different types.");
                    code_afterPPI.back().str += i.str;
                    i = code_afterPPI.back();
                    code_afterPPI.pop_back();
                    currinst = ppi_t::NewLine;
                    goto defcheck;
                case ppi_t::ppDef:
                    switch (i.type) {
                        case ppi_t::ppdef_name:
                            temp[0] = i.str;
                            def = Definition();
                            if (definitions.find(i.str) != definitions.end()) {
                                throw Exception("AlreadyDefinedError","Cannot redefine \"" + i.str +"\".");
                            }
                            goto next;
                        case ppi_t::ppdef_arg:
                            def.args.push_back(i.str);
                            goto next;
                        case ppi_t::ppdef_value_start:
                            currinst = ppi_t::ppdef_value_start;
                            goto next;
                        default:
                            throw Exception("UnexpectedToken","Unexpected token after #define.");
                    }
                case ppi_t::ppdef_value_start:
                    switch (i.type) {
                        case ppi_t::ppdef_value_end:
                            definitions[temp[0].s] = def;
                            currinst = ppi_t::NewLine;
                            goto next;
                        default:
                            def.val.push_back(i);
                            goto next;
                    }
                case ppi_t::ppUndef:
                    switch (i.type) {
                        case ppi_t::ppdef_name:
                            if (definitions.find(i.str) != definitions.end()) {
                                definitions.erase(i.str);
                            } else {
                                logger.warn("Undefinition of undefined macro \"" + i.str + "\".");
                            }
                            goto next;
                        default:
                            throw Exception("UnexpectedToken","Unexpected token after #undef.");
                    }
                case ppi_t::ppIf:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto parse;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppIfdef:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            if (ifexpr_parse.size() > 1) throw Exception("UnexpectedPreprocessorArgument","Unexpected preprocessor argument after \"#ifdef\".");
                            if (definitions.find(ifexpr_parse[0].str) != definitions.end()) {
                                logger.debug("Preprocessor definiton for \"" + ifexpr_parse[0].str + "\" found in #ifdef block.");
                            } else {
                                logger.debug("Preprocessor definiton for \"" + ifexpr_parse[0].str + "\" not found in #ifdef block.");
                                ifexpr_eval = 1;
                            }
                            goto next;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppIfndef:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            if (ifexpr_parse.size() > 1) throw Exception("UnexpectedPreprocessorArgument","Unexpected preprocessor argument after \"#ifndef\".");
                            if (definitions.find(ifexpr_parse[0].str) == definitions.end()) {
                                logger.debug("Preprocessor definiton for \"" + ifexpr_parse[0].str + "\" found in #ifndef block.");
                                ifexpr_eval = 1;
                            } else {
                                logger.debug("Preprocessor definiton for \"" + ifexpr_parse[0].str + "\" not found in #ifndef block.");
                            }
                            goto next;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppPragma:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto pragmaparse;
                        default:
                            ifexpr_parse.push_back(i);
                    }
                case ppi_t::ppError:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto errorparse;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppWarning:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto warnparse;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppInclude:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto includeparse;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppOpDef:
                    switch (i.type) {
                        case ppi_t::ppEndExpr:
                            currinst = ppi_t::NewLine;
                            goto opdefparse;
                        default:
                            ifexpr_parse.push_back(i);
                            goto next;
                    }
                case ppi_t::ppElse:
                    switch (i.type) {
                        case ppi_t::ppIf:
                        case ppi_t::ppIfdef:
                        case ppi_t::ppIfndef:
                            elseexpr_eval++;
                            goto next;
                        case ppi_t::ppEndif:
                            if (elseexpr_eval > 0) 
                                elseexpr_eval--;
                            else 
                                currinst = ppi_t::NewLine;
                        default:
                            goto next;
                            
                    }
                case ppi_t::ppdef_arg:
                    switch (def_stg) {
                        case def_arg_stg::waiting_start:
                            if (i.type != ppi_t::groupStart || i.str != "(") {
                                throw Exception("UnexpectedToken","Unexpected token after macro call.");
                            }
                            def_stg = def_arg_stg::next_arg;
                            def_args.push_back(std::vector<ppi>());
                            goto next;
                        case def_arg_stg::next_arg:
                            switch (i.type) {
                                case ppi_t::groupStart:
                                    def_args.back().push_back(i);
                                    defarg_index++;
                                    goto next;
                                case ppi_t::groupEnd:
                                    if (--defarg_index == 0) {
                                        logger.debug("Begin paring macro \"" + defname + "\"");
                                        auto out = defadr->parse(def_args);
                                        def_args.clear();
                                        defstack.push_back(defname);
                                        for (auto j : out) 
                                            code_runPPI.push_back(j);
                                        
                                        build_stage_4();
                                        defstack.pop_back();
                                        logger.debug("End parsing macro \"" + defname + "\"");
                                        goto next;
                                    } else {
                                        def_args.back().push_back(i);
                                        goto next;
                                    }
                                case ppi_t::Delimiter:
                                    if (i.str == "," && defarg_index == 0) {
                                        def_args.push_back(std::vector<ppi>());
                                        goto next;
                                    } else {
                                        def_args.back().push_back(i);
                                    }
                                default:
                                    def_args.back().push_back(i);
                                    goto next;
                            }
                    }
                case ppi_t::String:
                    if (i.type != ppi_t::Token)
                        throw Exception("UnexpectedToken","Unexpected character after transform to string literal.");
                    i = ppi(ppi_t::String, i.str);
                    code_afterPPI.push_back(i);
                    goto next;
                default: break;
            }
            goto next;
            parse:
            ifexpr_parse.clear();
            logger.warn("#if #elif not implemented, returning false");
            ifexpr_eval++;
            goto next;
            pragmaparse:
            if (ifexpr_parse.size() == 0) {
                logger.warn("#pragma without arguments, ignoring.");
                goto next;
            }
            if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::Token) goto unknownpragma;
                if (ifexpr_parse[0].str == "once") {
                    includedalready.back().reuseavail = false;
                    ifexpr_parse.clear();
                    goto next;
                } else if (ifexpr_parse[0].str == "nobase") {
                    nobase = true;
                    if (definitions.find("__NOBASE__") != definitions.end()) {
                        addDef("__NOBASE__",{},{});
                    }
                    logger.info("\"#pragma nobase\" found, will not load base language.");
                    ifexpr_parse.clear();
                    goto next;
                } else goto unknownpragma;
            } else {
                unknownpragma:
                logger.warn("Unknown pragma \"" + ifexpr_parse[0].str + "\".");
                ifexpr_parse.clear();
                goto next;
            }
            errorparse:
            if (ifexpr_parse.size() == 0) {
                logger.error("#error directive without arguments");
                throw Exception("ErrorDirective","#error directive (no args)");
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownerror;
                logger.error("#error directive: \"" + ifexpr_parse[0].str + "\"");
                throw Exception("ErrorDirective","#error directive (\"" + ifexpr_parse[0].str + "\")");
            } else {
                unknownerror:
                logger.warn("Unknown #error directive.");
                throw Exception("ErrorDirective","#error directive (unknown)");
            }
            warnparse:
            if (ifexpr_parse.size() == 0) {
                logger.warn("#warning directive without arguments");
                ifexpr_parse.clear();
                goto next;
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownwarn;
                logger.warn("#warning directive: \"" + ifexpr_parse[0].str + "\"");
                ifexpr_parse.clear();
                goto next;
            } else {
                unknownwarn:
                logger.warn("Unknown #warning directive.");
                ifexpr_parse.clear();
                goto next;
            }
            includeparse:
            if (ifexpr_parse.size() == 0) {
                logger.error("#include directive without arguments");
                throw Exception("UnexpectedPreprocessorArgument","#include directive (no args)");
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknowninclude;
                logger.debug("Including file " + ifexpr_parse[0].str);
                includefile(ifexpr_parse[0].str);
                ifexpr_parse.clear();
            } else {
                unknowninclude:
                logger.error("Unknown #include argument(s).");
                goto next;
            }
            opdefparse:
            if (ifexpr_parse.size() == 0) {
                logger.error("#opdef directive without arguments");
                throw Exception("UnexpectedPreprocessorArgument","#opdef directive (no args)");
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownopdef;
                logger.debug("Defining operator " + ifexpr_parse[0].str);
                addOp(ifexpr_parse[0].str);
                ifexpr_parse.clear();
                goto next;
            } else if (ifexpr_parse.size() == 2) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownopdef;
                if (ifexpr_parse[1].type != ppi_t::Token) goto unknownopdef;
                signed short c;
                try {
                    c = std::stoi(ifexpr_parse[1].str);
                } catch (...) {
                    goto unknownopdef;
                }
                logger.debug("Defining operator " + ifexpr_parse[0].str + " with priority(lower higher) " + ifexpr_parse[1].str);
                addOp(ifexpr_parse[0].str,c);
                ifexpr_parse.clear();
                goto next;
            } else if (ifexpr_parse.size() == 3) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownopdef;
                if (ifexpr_parse[1].type != ppi_t::Token) goto unknownopdef;
                if (ifexpr_parse[2].type != ppi_t::String) goto unknownopdef;
                signed short c;
                try {
                    c = std::stoi(ifexpr_parse[1].str);
                } catch (...) {
                    goto unknownopdef;
                }
                logger.debug("Defining operator " + ifexpr_parse[0].str + " with priority(highest first) " + ifexpr_parse[1].str);
                op o = op(ifexpr_parse[0].str,c);
                auto& at = o.attr;
                for (auto i : split(ifexpr_parse[2].str,',')) 
                    at.push_back(i);
                ifexpr_parse.clear();
                goto next;
            } else {
                unknownopdef:
                logger.error("Unknown #opdef argument(s).");
                ifexpr_parse.clear();
                goto next;
            }
            next:
            (void)0;
        }
        logger.debug("Running preprocessor... done");
    }
    void build_stage_5() {
        logger.debug("Parsing integer literals...");
        auto oldcode = code_afterPPI;
        code_afterPPI.clear();
        for (auto i : oldcode) {
            switch (i.type) {
                case ppi_t::Token:
                    switch (i.str[0]) {
                        case '0':
                            if (startsWith(i.str,"0x")) {
                                //make sure i.substr(2) is a hex number
                                if (i.str.size() == 2) {
                                    logger.error("Invalid hex literal: 0x");
                                    throw Exception("InvalidHexLiteral","Invalid hex literal: 0x");
                                }
                                for (size_t j = 2; j < i.str.size(); j++) {
                                    if (!isxdigit(i.str[j])) {
                                        logger.error("Invalid hex literal: " + i.str);
                                        throw Exception("InvalidHexLiteral","Invalid hex literal: " + i.str);
                                    }
                                }
                                code_numliteral.push_back(ppi(ppi_t::Integer,to_string(stoll(i.str.substr(2),nullptr,16))));
                            } else if (startsWith(i.str,"0b")) {
                                //make sure i.substr(2) is a binary number
                                if (i.str.size() == 2) {
                                    logger.error("Invalid binary literal: 0b");
                                    throw Exception("InvalidBinaryLiteral","Invalid binary literal: 0b");
                                }
                                for (size_t j = 2; j < i.str.size(); j++) {
                                    if (i.str[j] != '0' && i.str[j] != '1') {
                                        logger.error("Invalid binary literal: " + i.str);
                                        throw Exception("InvalidBinaryLiteral","Invalid binary literal: " + i.str);
                                    }
                                }
                                code_numliteral.push_back(ppi(ppi_t::Integer,to_string(stoll(i.str.substr(2),nullptr,2))));
                            } else if (startsWith(i.str,"0o")) {
                                //make sure i.substr(2) is an octal number
                                if (i.str.size() == 2) {
                                    logger.error("Invalid octal literal: 0o");
                                    throw Exception("InvalidOctalLiteral","Invalid octal literal: 0o");
                                }
                                for (size_t j = 2; j < i.str.size(); j++) {
                                    auto c = i.str[j];
                                    if (c < '0' || c > '7') {
                                        logger.error("Invalid octal literal: " + i.str);
                                        throw Exception("InvalidOctalLiteral","Invalid octal literal: " + i.str);
                                    }
                                }
                                code_numliteral.push_back(ppi(ppi_t::Integer,to_string(stoll(i.str.substr(2),nullptr,8))));
                            } else {
                                //make sure i.substr(1) is a decimal number
                                for (size_t j = 1; j < i.str.size(); j++) {
                                    if (!isdigit(i.str[j])) {
                                        logger.error("Invalid decimal literal: " + i.str);
                                        throw Exception("InvalidDecimalLiteral","Invalid decimal literal: " + i.str);
                                    }
                                }
                                code_numliteral.push_back(ppi(ppi_t::Integer,to_string(stoll(i.str.substr(1),nullptr,10))));
                            }
                            goto next;
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            //make sure i is a decimal number
                            for (size_t j = 0; j < i.str.size(); j++) {
                                if (!isdigit(i.str[j])) {
                                    logger.error("Invalid decimal literal: " + i.str);
                                    throw Exception("InvalidDecimalLiteral","Invalid decimal literal: " + i.str);
                                }
                            }
                            code_numliteral.push_back(ppi(ppi_t::Integer,i.str));
                            goto next;
                        default:
                            code_numliteral.push_back(i);
                            goto next;
                    }
                default:
                    code_numliteral.push_back(i);
                    goto next;
            }
            next:
            (void)0;
        }
        logger.debug("Parsing integer literals... done");
    }
    void build_stage_6() {
        logger.debug("Parsing...");
        auto oldcode = code_numliteral;
        code_numliteral.clear();
        code_stg6.setType(Instruction_t::InitCodeObject);
        enum class bl {
            none,
            cls,
            clsbase,
            clsbody,
            clstemplate,
            func,
            funcbody,   
            var,
            set,
            label,
            namesp,
            namespbody,
            enum_,
            enumbody,
            expression,
            attr
        };
        class blr_ {
            public:
            std::vector<bl> blv;
            Compiler* co;
            bool isInit() {
                AST& c = co->current;
                while (true) {
                    auto t = c.getType();
                    if (t == Instruction_t::CodeObject) return false;
                    else if (t == Instruction_t::InitCodeObject) return true;
                    c = *(c.getParent());
                }
            }
            bl get() {
                return blv.back();
            }
            void push(bl v) {
                blv.push_back(v);
            }
            void push(ppi v) {
                buf.push_back(v);
            }
            void pop() {
                blv.pop_back();
            }
            blr_() {
                blv.push_back(bl::none);
            }
            std::vector<std::string> attrs;
            std::vector<ppi> buf;
            unsigned short attrbraces = 0;
            unsigned short braces1 = 0;
            unsigned short braces2 = 0;
            unsigned short braces3 = 0;
            unsigned char defstg = 0; //class,namespace,function,etc.
            bool parsedexpr = false;
            void clear() {
                attrs.clear();
                buf.clear();
            }
            void pushattrs() {
                for (std::string i : attrs) co->ast_addAttr(i);
                attrs.clear();
            }
            void pushattr(const std::string& s) {
                attrs.push_back(s);
            }
        } blr;
        blr.co = this;
        for (auto i : oldcode) {
            reset:
            switch (blr.get()) {
                case bl::none:
                    if (i == ppi(ppi_t::Token,"__attr__")) {
                        blr.push(bl::attr);
                        goto next;
                    } else if (i.type == ppi_t::Token) {
                        if (i.str == "class") {
                            blr.push(bl::cls);
                            ast_new___(Instruction_t::Classdef);
                            goto next;
                        } else if (i.str == "namespace") {
                            blr.push(bl::namesp);
                            ast_new___(Instruction_t::Classdef);
                            goto next;
                        } else if (i.str == "enum") {
                            blr.push(bl::enum_);
                            ast_new___(Instruction_t::Classdef);
                            goto next;
                        } else {
                            blr.push(bl::var);
                            blr.push(i);
                            goto next;
                        }
                    }
                case bl::attr:
                    switch (blr.defstg) {
                        case 0:
                            if (i != ppi(ppi_t::groupStart,"(")) throw Exception(
                                "UnexpectedToken",
                                "Unexpected token after __attr__"
                            );
                            blr.defstg++;
                            goto next;
                        case 1:
                            if (i.type != ppi_t::Token && i.type != ppi_t::String) throw Exception(
                                "UnexpectedToken",
                                "Unexpected token after __attr__("
                            );
                            blr.defstg++;
                            blr.pushattr(i.str);
                            goto next;
                        case 2:
                            if (i == ppi(ppi_t::Delimiter,",")) {
                                blr.defstg = 1;
                                goto next;
                            } else if (i == ppi(ppi_t::groupEnd,")")) {
                                blr.defstg = 0;
                                blr.pop();
                                goto next;
                            } else throw Exception(
                                "UnexpectedToken",
                                "UnexpectedToken after attr"
                            );
                    }
                case bl::var:
                switch (blr.defstg++) {
                    case 0:
                        if (i.type == ppi_t::Operator) {
                            if (isInVector(blr.attrs,(std::string)"opfunc")) {
                                blr.pop();
                                blr.defstg = 0;
                                blr.push(bl::func);
                                goto reset;
                            }
                            auto opv = def_ops.findbyattr("set");
                            if (endsWith(i.str, opv.opr)) {
                                blr.defstg = 0;
                                blr.pop();
                                blr.push(bl::set);
                                blr.push(bl::expression);
                                ast_newOperator(i.str);
                                ast_addAttr("set");
                                blr.pushattrs();
                                ast_newToken(blr.buf[0]);
                                ast_back();
                                blr.clear();
                                goto next;
                            } else if (i.str == ":") {
                                blr.pop();
                                blr.defstg = 0;
                                ast_new__(Instruction_t::Label,blr.buf[0]);
                                blr.clear();
                                ast_back();
                                goto next;
                            }
                        }
                        if (i.type != ppi_t::Token) throw Exception(
                            "UnexpectedToken",
                            "Expected a token after variable definition, got " + (std::string)i
                        );
                        blr.push(i);
                        goto next;
                    case 1:
                        if (i == ppi(ppi_t::groupStart,"(")) {
                            blr.pop();
                            blr.push(bl::func);
                            ast_new__(Instruction_t::Funcdef, i.str);
                            blr.pushattrs();
                            ast_newClass(blr.buf[0]);
                            ast_back();
                            ast_new___(Instruction_t::FuncArgs);
                            blr.clear();
                            goto next;
                        } else if (i.type == ppi_t::Operator) {
                            auto opv = def_ops.findbyattr("set");
                            if (endsWith(i.str, opv.opr)) {
                                blr.push(bl::expression);
                                ast_new___(Instruction_t::Allocate);
                                ast_newClass(blr.buf[0].str);
                                ast_back();
                                ast_newToken(blr.buf[1].str);
                                ast_back();
                                ast_back();
                                ast_newOperator(i.str);
                                ast_addAttr("set");
                                blr.pushattrs();
                                ast_newToken(blr.buf[1]);
                                ast_back();
                                blr.clear();
                                goto next;
                                
                            }
                        } else throw Exception(
                            "UnexpectedToken",
                            "Expected a token after variable definition, got " + (std::string)i
                        );
                    default:
                        blr.pop();
                        ast_back();
                        blr.defstg = 0;
                        goto next;
                }
                case bl::set:
                    blr.pop();
                    ast_back();
                    goto next;
                case bl::func:
                switch (blr.defstg++) {
                    case 0:
                        ast_new__(Instruction_t::Funcdef, i.str);
                        blr.pushattrs();
                        ast_newClass(blr.buf[0]);
                        blr.clear();
                        ast_back();
                        goto next;
                    case 1:
                        if (i != ppi(ppi_t::groupStart,"(")) throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after function definition."
                        );
                        ast_new___(Instruction_t::FuncArgs);
                        goto next;
                    case 2:
                        if (i.type != ppi_t::Token) throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after function arguments definition."
                        );
                        blr.push(i);
                        goto next;
                    case 3:
                        if (i.type != ppi_t::Token) throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after function arguments definition."
                        );
                        ast_newToken(i.str);
                        ast_newClass(blr.buf[0]);
                        blr.clear();
                        ast_back();
                        ast_back();
                        goto next;
                    case 4:
                        if (i == ppi(ppi_t::groupEnd,")")) {
                            ast_back();
                        } else if (i == ppi(ppi_t::Delimiter,",")) {
                            blr.defstg = 2;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after function argument definition."
                        );
                        goto next;
                    case 5:
                        if (i == ppi(ppi_t::groupStart, "{")) {
                            blr.pop();
                            blr.push(bl::funcbody);
                            blr.clear();
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::Delimiter, ";")) {
                            ast_back();
                            blr.clear();
                            blr.pop();
                            blr.defstg = 0;
                            goto next;
                        }
                        else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after function general definition"
                        );
                }
                case bl::cls:
                switch (blr.defstg++) {
                    case 0:
                       ast_setToken(i.str);
                       blr.pushattrs();
                       blr.clear();
                       goto next;
                    case 1:
                        if (i == ppi(ppi_t::Delimiter,";")) {
                            ast_back();
                            blr.pop();
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::Operator, ":")) {
                            blr.pop();
                            blr.push(bl::clsbase);
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::groupStart,"{")) {
                            blr.pop();
                            blr.push(bl::clsbody);
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::Operator,"<")) {
                            ast_new___(Instruction_t::Template);
                            blr.push(bl::clstemplate);
                            blr.defstg = 0;
                            goto next;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after start of class definition."
                        );
                }
                case bl::clsbase:
                    if (i == ppi(ppi_t::Token,"__attr__")) {
                        blr.push(bl::attr);
                        goto next;
                    } else if (i.type == ppi_t::Token) {
                        ast_new__(Instruction_t::Classbase,i.str);
                        blr.pushattrs();
                        ast_back();
                        goto next;
                    } else if (i == ppi(ppi_t::groupStart,"{")) {
                        blr.pop();
                        blr.push(bl::clsbody);
                        goto next;
                    } else throw Exception(
                        "UnexpectedToken",
                        "Unexpected token after class base."
                    );
                case bl::clstemplate:
                switch (blr.defstg++) {
                    case 0:
                        if (i.type == ppi_t::Token) {
                            blr.push(i);
                            goto next;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after class template"
                        );
                    case 1:
                        if (i.type == ppi_t::Token) {
                            ast_newToken(i.str);
                            ast_newClass(blr.buf[0].str);
                            ast_back();
                            ast_back();
                            blr.clear();
                            goto next;
                        } else if (i == ppi(ppi_t::Delimiter,",")) {
                            ast_newToken(blr.buf[0].str);
                            ast_back();
                            blr.clear();
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::Operator,">")) {
                            ast_newToken(blr.buf[0].str);
                            ast_back();
                            blr.clear();
                            blr.defstg = 3;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after class template"
                        );
                    case 2:
                        if (i == ppi(ppi_t::Delimiter,",")) {
                            blr.defstg = 0;
                            goto next;
                        } else if (i == ppi(ppi_t::Operator,">")) {
                            ast_back();
                            goto next;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after class template"
                        );
                    case 3:
                        if (i == ppi(ppi_t::Operator,":")) {
                            blr.clear();
                            blr.pop();
                            blr.push(bl::clsbase);
                            goto next;
                        } else if (i == ppi(ppi_t::groupStart,"{")) {
                            blr.clear();
                            blr.pop();
                            blr.push(bl::clsbody);
                            goto next;
                        } else if (i == ppi(ppi_t::Delimiter,";")){
                            ast_back();
                            blr.pop();
                            blr.clear();
                            goto next;
                        } else throw Exception(
                            "UnexpectedToken",
                            "Unexpected token after class template"
                        );
                        
                }
                case bl::funcbody:
                    if (i.type == ppi_t::Token) {
                        
                    }
            }
            next:
            (void)0;
        }
        logger.debug("Parsing... done");
    }
    bool verbose = false;
    void build(std::string code) {
        logger.begin();
        logger.info("Begin build for main.");
        includestack.push_back("<main>");
        srand(time(NULL));
        build_stage_1(code);
        build_stage_2();
        build_stage_3();
        build_stage_4();
        if (!nobase) {
            std::vector<ppi> t = code_afterPPI;
            code_afterPPI.clear();
            includefile("baselang.meh");
            code_runPPI = t;
            build_stage_4();
        }
        build_stage_5();
        build_stage_6();
        // etc.
        logger.info("Finish build for main.");
        includestack.pop_back();

    }
    std::string exportdata() {
        return code_stg6.repr();
    }
    Compiler() : logger(Logger(*this)) {
        definitions["FOR_EACH"] = ForEveryDefinition();
        definitions["GET_T_DEF"] = GetTDefDefinition();
        definitions["HAS_ARGS_DEF"] = HasArgsDefinition();
    };
};