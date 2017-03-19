#include <assert.h>
#include <string.h>

#include "kmp.h"

int main(int argc, char *argv[])
{
    size_t rc;
    char *key = "12";
    char *str = "11111111121111";

    rc = kmp(key, strlen(key), str, strlen(str));
    assert(rc == 8);

    return 0;
}

