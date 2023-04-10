// SPDX-License-Identifier: MIT
// gcc -O3 spacer.c -o spacer
// gcc --version 11.3.0 on soho ubuntu 22.04

#include <stdbool.h>

bool spacer(char c) {
    return c == ' '
        || c == '\r'
        || c == '\n'
        || c == '\t';
}

int main() {
    spacer(' ');
}