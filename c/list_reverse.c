// SPDX-License-Identifier: MIT
//
// gcc -O3 list_reverse.c -o list_reverse
// gcc --version 11.3.0 on soho ubuntu 22.04
//
// clang -O3 list_reverse.c -o list_reverse
// Apple clang version 14.0.0 (clang-1400.0.29.202) on Apple M1 macOS Ventura 13.2.1
//
// %comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
// cl /EHsc list_reverse.c
// cl version 19.35.32215 for x64

#include "common.h"

typedef struct node {
    struct node *next;
    int data;
} node_t;

// A --> B ---> C --> NULL
// C --> B ---> A --> NULL

struct node *reverse(struct node *head) {
    struct node *prev, *next;
    
    if (!head)
        return NULL;
    
    if (!head->next)
        return head;
    
    prev = head;
    head = head->next;
    prev->next = NULL;
    
    while (head) {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    
    return prev;
}

void reverse0(void) {
    struct node *R;

    printf("Original List: NULL\n");

    R = reverse(NULL);
    if (R)
        printf("Reversed List: %p (ERROR)\n", R);
    else
        printf("Reversed List: NULL\n");
}

void reverse1(void) {
    struct node *A, *R;

    A = malloc(sizeof(struct node));

    if (A) {
        A->next = NULL;
        printf("Original List: %p --> ", A);
        printf("NULL\n");

        R = reverse(A);
        if (R) {
            printf("Reversed List: %p --> ", R);
            if (R->next == NULL)
                printf("NULL (OK)");
            else
                printf("%p --> (ERROR)", R->next);
            printf("\n");
        }
    }

    if (A) free(A);
}

void reverse2(void) {
    struct node *A, *B, *R;

    A = malloc(sizeof(struct node));
    B = malloc(sizeof(struct node));

    if (A && B) {
        A->next = B;
        B->next = NULL;
        printf("Original List: %p --> ", A);
        printf("%p --> ", A->next);
        printf("NULL\n");

        R = reverse(A);
        if (R) {
            printf("Reversed List: %p --> ", R);
            if (R->next == A) {
                printf("%p --> ", R->next);
                if (R->next->next == NULL)
                    printf("NULL (OK)");
                else
                    printf("%p --> (ERROR)", R->next->next);
            }
            printf("\n");
        }
    }

    if (A) free(A);
    if (B) free(B);
}

void reverse3(void) {
    struct node *A, *B, *C, *R;

    A = malloc(sizeof(struct node));
    B = malloc(sizeof(struct node));
    C = malloc(sizeof(struct node));

    if (A && B && C) {
        A->next = B;
        B->next = C;
        C->next = NULL;
        printf("Original List: %p --> ", A);
        printf("%p --> ", A->next);
        printf("%p --> ", A->next->next);
        printf("NULL\n");

        R = reverse(A);
        if (R) {
            printf("Reversed List: %p --> ", R);
            if (R->next == B) {
                printf("%p --> ", R->next);
                if (R->next->next == A) {
                    printf("%p --> ", R->next->next);
                    if (R->next->next->next == NULL)
                        printf("NULL (OK)");
                    else
                        printf("%p --> (ERROR)", R->next->next->next);
                }
            }
            printf("\n");
        }
    }

    if (A) free(A);
    if (B) free(B);
    if (C) free(C);
}

void reverse4(void) {
    struct node *A, *B, *C, *D, *R;

    A = malloc(sizeof(struct node));
    B = malloc(sizeof(struct node));
    C = malloc(sizeof(struct node));
    D = malloc(sizeof(struct node));

    if (A && B && C && D) {
        A->next = B;
        B->next = C;
        C->next = D;
        D->next = NULL;
        printf("Original List: %p --> ", A);
        printf("%p --> ", A->next);
        printf("%p --> ", A->next->next);
        printf("%p --> ", A->next->next->next);
        printf("NULL\n");

        R = reverse(A);
        if (R) {
            printf("Reversed List: %p --> ", R);
            if (R->next == C) {
                printf("%p --> ", R->next);
                if (R->next->next == B) {
                    printf("%p --> ", R->next->next);
                    if (R->next->next->next == A) {
                        printf("%p --> ", R->next->next->next);
                        if (R->next->next->next->next == NULL)
                            printf("NULL (OK)");
                        else
                            printf("%p --> (ERROR)", R->next->next->next->next);
                    }
                }
            }
            printf("\n");
        }
    }

    if (A) free(A);
    if (B) free(B);
    if (C) free(C);
    if (D) free(D);
}

int main(int argc, char *argv[]) {
    reverse0();
    reverse1();
    reverse2();
    reverse3();
    reverse4();
    return 0;
}
