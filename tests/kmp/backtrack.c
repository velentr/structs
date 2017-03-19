#include <assert.h>
#include <string.h>

#include "kmp.h"

int main(int argc, char *argv[])
{
    size_t rc;
    char *key = "123";
    char *str = "12123121212";

    rc = kmp(key, strlen(key), str, strlen(str));
    assert(rc == 2);

    return 0;
}

