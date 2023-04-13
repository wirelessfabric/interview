// SPDX-License-Identifier: MIT
//
// gcc -O3 spacer.c -o spacer
// gcc --version 11.3.0 on soho ubuntu 22.04
//
// clang -O3 spacer.c -o spacer
// Apple clang version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc spacer.c
// cl version 19.35.32215 for x64

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