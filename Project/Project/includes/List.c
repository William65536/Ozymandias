#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "List.h"
#include "ListPreamble.h"

#define concat(a, b) a##b
#define tricat(a, b, c) a##b##c
#define typename(T) concat(T, List)
#define funcname(type, func) tricat(type, _, func)

/** TODO: Allow for the use of custom allocators */

struct typename(T) {
    size_t size, cap;
    T data[];
};

typename(T) *funcname(typename(T), new)(size_t initcap)
{
    assert(initcap > 0);

    typename(T) *ret = malloc(sizeof *ret + initcap * sizeof *ret->data);

    if (ret == NULL)
        return NULL;

    ret->size = 0;
    ret->cap = initcap;

    return ret;
}

void funcname(typename(T), delete)(typename(T) **self)
{
    assert(self != NULL && *self != NULL);

    free(*self);
    *self = NULL;
}

size_t funcname(typename(T), size)(const typename(T) *self)
{
    assert(self != NULL);

    return self->size;
}

T funcname(typename(T), get)(const typename(T) *self, size_t i)
{
    assert(self != NULL);
    assert(i < self->size);

    return self->data[i];
}

const T *funcname(typename(T), begin)(const typename(T) *self)
{
    assert(self != NULL);

    return self->data;
}

const T *funcname(typename(T), end)(const typename(T) *self)
{
    assert(self != NULL);

    return self->data + self->size;
}

bool funcname(typename(T), resize)(typename(T) **self) 
{
    assert(self != NULL && *self != NULL);

    typename(T) *temp = realloc(*self, sizeof **self + (*self)->cap * sizeof *(*self)->data * 2);

    if (temp == NULL)
        return false;

    *self = temp;
    (*self)->cap *= 2;

    return true;
}

bool funcname(typename(T), push)(typename(T) **self, T value)
{
    assert(self != NULL && *self != NULL);

    if ((*self)->size + 1 > (*self)->cap)
        if (!funcname(typename(T), resize)(self))
            return false;

    (*self)->data[(*self)->size] = value;
    (*self)->size++;

    return true;
}
