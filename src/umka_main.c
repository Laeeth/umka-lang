#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "umka_compiler.h"
#include "umka_expr.h"
#include "umka_decl.h"


Compiler comp;


void compileError(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char msg[256];
    vsprintf(msg, format, args);
    printf("Error %s (%d, %d): %s\n", comp.lex.fileName, comp.lex.line, comp.lex.pos, msg);

    compilerFree(&comp);
    exit(1);

    va_end(args);
}


void runtimeError(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char msg[256];
    vsprintf(msg, format, args);

    Instruction *instr = &comp.vm.fiber->code[comp.vm.fiber->ip];
    printf("Runtime error %s (%d): %s\n", instr->debug.fileName, instr->debug.line, msg);

    compilerFree(&comp);
    exit(1);

    va_end(args);
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: umka <file.um>\n");
        return 1;
    }

    compilerInit    (&comp, argv[1], 1024 * 1024, argc, argv, compileError);
    compilerCompile (&comp);
/*
    char assembly[100000];
    compilerAsm(&comp, assembly);
    printf("%s", assembly);
*/
    compilerRun     (&comp, 1024 * 1024, runtimeError);
    compilerFree    (&comp);

    return 0;
}
