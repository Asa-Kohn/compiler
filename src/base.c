#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct array
{
    size_t len;
    union obj *data;
    int refcount;
};

struct slice
{
    struct array *data;
    size_t len;
    size_t cap;
};

union obj
{
    struct array a;
    struct slice s;
    union obj *st;
    int i;
    double f;
    char *str;
};

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
    char *c = emalloc(strlen(s) + 1);
    strcpy(c, s);
    return c;
}

char *zerostr()
{
    char *str = emalloc(1);
    *str = '\0';
    return str;
}

struct array zeroarray(int len)
{
    union obj *data = malloc(sizeof(union obj) * len);
    return (struct array) {.len = len, .data = data, .refcount = 0};
}

struct slice zeroslice()
{
    struct array *a = emalloc(sizeof(struct array));
    *a = zeroarray(0);
    return (struct slice) {.data = a, .len = 0, .cap = 0};
}
