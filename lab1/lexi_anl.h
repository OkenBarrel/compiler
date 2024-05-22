#pragma once
#include "lexi_class.h"
#include <queue>
#include <string>
#define IS_LETTER 0
#define IS_DIGIT 1
#define IS_SYMBOL 2

enum TOKEN{
    DEC,
    ILDEC,
    OCT,
    ILOCT,
    HEX,
    ILHEX,
    IDN,//6
    ADD,
    SUB,
    MUL,
    DIV,
    GT,
    LT,
    EQ,
    GE,
    LE,
    NEQ,
    SLP,
    SRP,
    SEMI,
    FUNC,
    ERROR
};
void hello();

symbolTableNode scan_digit(std::string in,int* error);

symbolTableNode scan_letter(std::string in);

symbolTableNode scan_else(std::string in,int* error);

std::queue<std::string> sep_words(std::string in);

int tellType(char ch);
