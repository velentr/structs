#include <assert.h>
#include <stdlib.h>

#include "blkalloc.h"

int main(int argc, char *argv[])
{
    struct blkalloc a;
    int rc;

    rc = blkalloc_init(&a, malloc, free, 0, 256);
    assert(rc < 0);

    return 0;
}
