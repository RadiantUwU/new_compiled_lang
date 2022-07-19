#include "compiler.hpp"

int main() {
    Compiler compiler;
    #ifndef NDEBUG
    compiler.verbose = true;
    #endif
    compiler.build("#ifndef NDEBUG\n#define NDEBUG\n#endif\n");
    return 0;
}