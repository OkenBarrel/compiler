#pragma once
// #include <iostream>
#include "lexi_class.h"
#define IS_LETTER 0
#define IS_DIGIT 1

void hello();

// void scan(std::string in);

symbolTableNode scan_digit(std::string in);

void scan_letter();

int tellType(char ch);
