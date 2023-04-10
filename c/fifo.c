// SPDX-License-Identifier: MIT
// gcc -O3 fifo.c -o fifo -lm -ltbb
// g++ --version 11.3.0 on soho ubuntu 22.04
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc fifo.c
// cl version 19.35.32215 for x64

#include <stdio.h>

#define BUFFER_SIZE 10

static struct {
    int write;
    int read;
    char data[BUFFER_SIZE];
} buffer;

void init_fifo() {
    buffer.write = 0;
    buffer.read = 0;
}

int read_fifo(char *out) {
    int read = buffer.read;
    if (read == buffer.write)
        return -1;

    int next = read + 1;
    if (next == BUFFER_SIZE)
        next = 0;

    char data = buffer.data[read];
    buffer.read = next;

    if (out)
        *out = data;
    return read;
}

int write_fifo(char data) {
    int write = buffer.write;
    int next = write + 1;
    if (next == BUFFER_SIZE)
        next = 0;

    if (next == buffer.read)
        return -1;

    buffer.data[write] = data;
    buffer.write = next;
    return write;
}

int drain_fifo(int r, int w) {
    while (r < w) {
        char data = 0x55;
        if (read_fifo(&data) < 0)
            break;
        r++;
    }
    return r;
}

int main(int argc, char *argv[]) {
    int count = 1;
    int w = 0;
    int r = 0;
 
    if (argc > 1) {
        sscanf(argv[1], "%d", &count);
    } else {
        printf("Input count of read/write ops: ");
        if (scanf("%d", &count) != 1) {
            printf("scanf() ERROR");
            return -1;
        }
    }

    printf("# of read/write ops: %d\n", count);

    init_fifo();

    while (w < count) {
        char data = (char)w;
        int rc = write_fifo(data);
        if (rc < 0) {
            r = drain_fifo(r, w);
            continue;
        }
        w++;
    }
    if (r < w)
        drain_fifo(r, w);

    return 0;
}
