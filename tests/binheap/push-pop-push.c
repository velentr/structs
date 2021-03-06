#include <stdlib.h>
#include <assert.h>

#include "binheap.h"

#ifndef TEST_SIZE
#define TEST_SIZE 1024
#endif

int cmp(const void *a, const void *b)
{
    const int *_a, *_b;

    _a = a;
    _b = b;

    return *_a - *_b;
}

int main(int argc, char *argv[])
{
    struct binheap uut;
    int i;
    long prev;

    binheap_init(&uut, cmp, sizeof(int), 0, realloc);

    i = 3;
    binheap_push(&uut, &i);
    i = 2;
    binheap_push(&uut, &i);
    i = 1;
    binheap_push(&uut, &i);

    assert(*(int*)binheap_peek(&uut) == 1);
    binheap_pop(&uut);

    i = 4;
    binheap_push(&uut, &i);

    assert(*(int*)binheap_peek(&uut) == 2);
    binheap_pop(&uut);
    assert(*(int*)binheap_peek(&uut) == 3);
    binheap_pop(&uut);
    assert(*(int*)binheap_peek(&uut) == 4);
    binheap_pop(&uut);

    binheap_destroy(&uut);

    return 0;
}
