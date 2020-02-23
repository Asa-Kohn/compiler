#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//emalloc throws an error if the attempt to allocate memory fail
void *emalloc(size_t n)
{
    void *result = malloc(n);
    if(!result)
    {
        fprintf(stderr, "Error: out of memory\n");
        exit(1);
    }
    return result;
}

char *estrdup(char *s)
{
    return strcpy(emalloc(strlen(s) + 1), s);
}
