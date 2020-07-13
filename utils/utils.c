#include "stdlib.h"

void shuffle_array(void *array, size_t n_elems, size_t elem_size)
{
    char tmp[elem_size];
    char *arr = array;
    size_t stride = elem_size * sizeof(char);

    if (n_elems > 1)
    {
        size_t i;
        for (i = 0; i < n_elems - 1; ++i)
        {
            size_t rnd = (size_t)rand();
            size_t j = i + rnd / (RAND_MAX / (n_elems - i) + 1);

            memcpy(tmp, arr + j * stride, elem_size);
            memcpy(arr + j * stride, arr + i * stride, elem_size);
            memcpy(arr + i * stride, tmp, elem_size);
        }
    }
}