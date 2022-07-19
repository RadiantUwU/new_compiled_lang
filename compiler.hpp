#pragma once

#include <ctime>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <initializer_list>
#include <fstream>

#include "data_types/instructions.hpp"
//#include "data_types/class.hpp" //buggy code, leave it here for now
#include "compiler_def_1.hpp"
#include "data_types/exception.hpp"
#include "headers/colors.hpp"
#include "headers/stringUtilities.hpp"
/*
    build_stage_1 is broken, escape sequences not handled
*/

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

            }
            ss << ", " << str << ")";
            return ss.str();
        }
    };
    class Definition {
        public:
        std::vector<std::string> args;
        std::vector<ppi> val;
        std::string parse() {

        }
        Definition(const std::vector<std::string>& args, const std::vector<ppi>& val) : args(args), val(val) {};
        Definition(const std::initializer_list<std::string>& args, const std::initializer_list<ppi>& val) : args(args), val(val) {};
        Definition() = default;
    };
    std::vector<std::string> includepath;
    bool nobase = false;
private:
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
    std::vector<std::string> codes;
    std::vector<std::string> includestack;
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
    std::unordered_map<std::string, Definition> definitions;
    std::vector<pppi> uncommentedcode;
    std::vector<ppi> code_interPPI;
    std::vector<ppi> code_runPPI;
    std::vector<ppi> code_afterPPI;
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
        enum class MsgType {
            OK,
            NotFound,
            AlreadyIncluded,
            CyclicInclude
        } msg;
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
                err = 0;
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
        ret.msg = (std::find(includedalready.begin(), includedalready.end(), std::hash<std::string>()(ret.code)) != includedalready.end()) ? IncludeFileOutput::MsgType::AlreadyIncluded : IncludeFileOutput::MsgType::OK;
        return ret;
    }
public:
    void addDef(const std::string& name, const std::vector<std::string>& args, const std::vector<ppi>& val) {
        definitions[name] = Definition(args, val);
    }
    void addDef(const std::string& name, const std::initializer_list<std::string>& args, const std::initializer_list<ppi>& val) {
        definitions[name] = Definition(args, val);
    }
    void build_stage_1(std::string code) {
        logger.debug("Uncommenting code and doing first token split...");
        std::string buffer;
        unsigned char c = 0;
        enum class bl {
            str,
            str_esc,
            chr,
            chr_esc,
            none,
            comment,
            block_comment
        } building = bl::none;
        code += '\n';
        for (auto ch : code) {
            switch (ch) {
                case '\r':
                    break;
                case ' ':
                case '\t':
                case '\n':
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
                        }
                        uncommentedcode.push_back(p);
                        buffer.clear();
                        if (ch == '\n') uncommentedcode.push_back(pppi(pppi_t::NewLine));
                    }
                    break;
                case '/':
                    if (building == bl::none) {
                        if (buffer.back() == '/') {
                            building == bl::comment;
                            buffer.pop_back();
                            if (buffer.size() > 0) {
                                uncommentedcode.push_back(pppi(buffer));
                                buffer.clear();
                            }
                        } else if (buffer.back() == '*') {
                            building == bl::block_comment;
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
                            building == bl::none;
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
            next: (void)0;
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
                }
                while (char_index < p.str.size()) {
                    char ch = *(p.str.begin() + char_index);
                    switch (ch) {
                        case '#':
                            building = bl::ppi_;
                            char_index++;
                            goto statementReset;
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
                    goto next;
                

            }
            switch (currinst) {
                case ppi_t::NewLine:
                    switch (i.type) {
                        case ppi_t::ppDef:
                        case ppi_t::ppIf:
                        case ppi_t::ppIfdef:
                        case ppi_t::ppIfndef:
                        case ppi_t::ppInclude:
                        case ppi_t::ppUndef:
                        case ppi_t::ppError:
                        case ppi_t::ppWarning:
                        case ppi_t::ppConcat:
                        case ppi_t::ppOpDef:
                        case ppi_t::ppPragma:
                            currinst = i.type;
                            goto next;
                        case ppi_t::ppElse:
                        case ppi_t::ppElif:
                        case ppi_t::ppElifdef:
                        case ppi_t::ppElifndef:
                            throw Exception("UnexpectedPreprocessor","Unexpected preprocessor instruction \"#" + i.str + "\".");
                        case ppi_t::ppEndif:
                            logger.debug("Endif token");
                            goto next;
                        case ppi_t::ppEndExpr:
                            logger.warn("Unexpected endexpr token.");
                            goto next;
                        default:
                            code_afterPPI.push_back(i);
                            goto next;
                    }
                case ppi_t::ppConcat:
                    if (i.type != code_afterPPI.back().type) throw Exception("InvalidConcatenation","Cannot concat two different types.");
                    code_afterPPI.back().str += i.str;
                    currinst = ppi_t::NewLine;
                    goto next;
                case ppi_t::ppDef:
                    switch (i.type) {
                        case ppi_t::ppdef_name:
                            temp[0] = i.str;
                            def = Definition();
                            goto next;
                        case ppi_t::ppdef_arg:
                            def.args.push_back(i.str);
                            goto next;
                        case ppi_t::ppdef_value_start:
                            currinst = ppi_t::ppdef_value_start;
                            goto next;
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
                    goto next;
                } else if (ifexpr_parse[0].str == "nobase") {
                    nobase = true;
                    if (definitions.find("__NOBASE__") != definitions.end()) {
                        addDef("__NOBASE__",{},{});
                    }
                    logger.info("\"#pragma nobase\" found, will not load base language.");
                    goto next;
                } else goto unknownpragma;
            } else {
                unknownpragma:
                logger.warn("Unknown pragma \"" + ifexpr_parse[0].str + "\".");
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
                goto next;
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknownwarn;
                logger.warn("#warning directive: \"" + ifexpr_parse[0].str + "\"");
                goto next;
            } else {
                unknownwarn:
                logger.warn("Unknown #warning directive.");
                goto next;
            }
            includeparse:
            if (ifexpr_parse.size() == 0) {
                logger.error("#include directive without arguments");
                throw Exception("UnexpectedPreprocessorArgument","#include directive (no args)");
            } else if (ifexpr_parse.size() == 1) {
                if (ifexpr_parse[0].type != ppi_t::String) goto unknowninclude;
                logger.debug("Including file " + ifexpr_parse[0].str);
                IncludeFileOutput in = include1(ifexpr_parse[0].str);
            } else {
                unknowninclude:
                logger.error("Unknown #include argument(s).");
                goto next;
            }
            next:
        }
    }
    bool verbose = false;
    void build(std::string code) {
        logger.begin();
        logger.info("Begin build for main.");
        includestack.push_back("main");
        build_stage_1(code);
        build_stage_2();
        build_stage_3();
        build_stage_4();
    }
    Compiler() : logger(Logger(*this)) {};
};