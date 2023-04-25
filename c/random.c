// SPDX-License-Identifier: MIT
//
// gcc -O3 random.c -o random -lcrypto
// gcc --version 11.3.0 on soho ubuntu 22.04
//
// clang -O3 random.c -o random
// Apple clang version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc random.c
// cl version 19.35.32215 for x64

// https://www.youtube.com/watch?v=wDj64pSeQ4I
// Is rand( ) Actually Random?

// https://www.youtube.com/watch?v=Gb-1grkVGSg
// rand Considered Harmful : Stephan T. Lavavej

#include "common.h"
#ifdef __linux__
#include <openssl/rand.h>
#endif

// https://www.youtube.com/watch?v=wDj64pSeQ4I @ 2:00
int random_seed(int seed) {
    srand(seed);
    return rand();
}

// https://www.youtube.com/watch?v=wDj64pSeQ4I @ 2:30
// https://www.youtube.com/watch?v=Gb-1grkVGSg @ 3:00
int random_time() {
    srand(time(NULL));
    return rand();
}

// https://www.youtube.com/watch?v=wDj64pSeQ4I @ 2:50
int random_nsec() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    srand(ts.tv_nsec);
    return rand();
}

// https://www.youtube.com/watch?v=wDj64pSeQ4I @ 3:50
#ifdef __linux__
int random_openssl() {
    unsigned char bytes[100];
    int wrap, i;

    RAND_bytes(bytes, sizeof(bytes));
    for (wrap=0, i=0; i < sizeof(bytes); i++) {
        printf("%02x ", bytes[i]);
        wrap++;
        if (wrap == 8 || wrap == 16 || wrap == 24) {
            printf(" ");
        } else if (wrap == 32) {
            wrap = 0;
            printf("\n");
        }
    }
    printf("\n");
    return 0;
}
#else
int random_openssl() {
    return random_nsec();
}
#endif

int main(int argc, char** argv) {
    int opt, random;

    opt = argc > 1 ? atoi(argv[1]) : 1;
    switch (opt) {
    case 0:
        random = random_openssl();
        break;
    case 1:
        random = random_nsec();
        break;
    case 2:
        random = random_time();
        break;
    case 3:
        random = random_seed(1337);
        break;
    default:
        random = random_seed(opt);
        break;
    }

    printf("%d\n", random);
    return random;
}
