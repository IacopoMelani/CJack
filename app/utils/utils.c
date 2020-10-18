#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear_screen()
{
#ifdef RELEASE
#ifdef __linux__
    system("clear");
#elif _WIN32
    system("cls");
#else
    // nothing :(
#endif
#endif
}

void shuffle_array(void *array, size_t n, size_t size)
{
    char tmp[size];
    char *arr = array;
    size_t stride = size * sizeof(char);

    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; ++i)
        {
            size_t rnd = (size_t)rand();
            size_t j = i + rnd / (RAND_MAX / (n - i) + 1);

            memcpy(tmp, arr + j * stride, size);
            memcpy(arr + j * stride, arr + i * stride, size);
            memcpy(arr + i * stride, tmp, size);
        }
    }
}