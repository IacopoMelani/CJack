#include "registry.h"

#include "malloc.h"

static struct RegistryAlloc *add_registry_alloc(struct RegistryAlloc *registry, size_t size, void *mem_alloc_ptr, const char *context);
static struct RegistryAlloc *create_registy_alloc(size_t size, void *mem_alloc_ptr, const char *context);
static struct RegistryAlloc *delete_registy_alloc(struct RegistryAlloc *registry, void *mem_alloc_ptr);
static void print_registy_alloc(struct RegistryAlloc *registry);

struct RegistryAlloc
{
    const char *context;
    size_t size;
    void *mem_alloc_ptr;
    struct RegistryAlloc *next;
};

struct RegistryAlloc *registry;

void registry_clear_alloc(void *mem_alloc_ptr)
{
    registry = delete_registy_alloc(registry, mem_alloc_ptr);
}

unsigned int registry_len(void)
{
    struct RegistryAlloc *pivot;
    unsigned int cap = 0;

    pivot = registry;

    if (pivot != NULL)
    {
        do
        {
            cap++;
            pivot = pivot->next;
        } while (pivot != NULL);
    }

    return cap;
}

size_t registry_total_size(void)
{
    struct RegistryAlloc *pivot;
    size_t tot = 0;

    pivot = registry;

    if (pivot != NULL)
    {
        while (pivot != NULL)
        {
            tot += pivot->size;
            pivot = pivot->next;
        }
    }

    return tot;
}

void registry_print_alloc(void)
{
    struct RegistryAlloc *pivot;

    pivot = registry;

    printf("\nTotal alloc: %u, Total size allocated: %ld", registry_len(), registry_total_size());

    while (pivot != NULL)
    {
        print_registy_alloc(pivot);
        pivot = pivot->next;
    }

    printf("\n");
}

bool registry_write_alloc(size_t size, void *mem_alloc_ptr, const char *context)
{
    if (registry == NULL)
    {
        registry = create_registy_alloc(size, mem_alloc_ptr, context);
    }
    else
    {
        registry = add_registry_alloc(registry, size, mem_alloc_ptr, context);
    }

    if (registry == NULL)
    {
        return false;
    }

    return true;
}

static struct RegistryAlloc *add_registry_alloc(struct RegistryAlloc *registry, size_t size, void *mem_alloc_ptr, const char *context)
{
    struct RegistryAlloc *pivot, *newRegistry;
    newRegistry = create_registy_alloc(size, mem_alloc_ptr, context);

    if (newRegistry != NULL)
    {
        pivot = registry;

        while (pivot->next != NULL)
        {
            pivot = pivot->next;
        }

        pivot->next = newRegistry;
    }

    return registry;
}

static struct RegistryAlloc *create_registy_alloc(size_t size, void *mem_alloc_ptr, const char *context)
{
    struct RegistryAlloc *registry = malloc(sizeof(struct RegistryAlloc));
    if (registry != NULL)
    {
        registry->context = context;
        registry->size = size;
        registry->mem_alloc_ptr = mem_alloc_ptr;
        registry->next = NULL;
    }

    return registry;
}

static struct RegistryAlloc *delete_registy_alloc(struct RegistryAlloc *registry, void *mem_alloc_ptr)
{
    if (registry == NULL)
    {
        return NULL;
    }

    if (registry->mem_alloc_ptr == mem_alloc_ptr)
    {
        struct RegistryAlloc *tempNextP;

        tempNextP = registry->next;

        free(registry);

        return tempNextP;
    }

    registry->next = delete_registy_alloc(registry->next, mem_alloc_ptr);

    return registry;
}

static void print_registy_alloc(struct RegistryAlloc *registry)
{
    printf("\n");
    printf("\n ---- ALLOC MEMORY LEAK %p ----", registry->mem_alloc_ptr);
    printf("\ncontext: %s", registry->context);
    printf("\nsize: %ld", registry->size);
}