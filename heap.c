#include "header.h"

int heap_init(t_heap *heap, int capacity)
{
    heap->tab = malloc(sizeof(t_request) * capacity);
    if (!heap->tab)
        return (1);
    heap->size = 0;
    heap->capacity = capacity;
    return (0);
}

static int  passes_before(t_request *a, t_request *b)
{
    if (a->key < b->key || (a->key == b->key && a->id < b->id))
        return (1);
    return (0);
}

int heap_push(t_heap *heap, t_request *request)
{
    t_request   tmp;
    int         size;
    t_request   *tab;

    if (heap->size == heap->capacity)
        return (1);
    tab = heap->tab;
    size = heap->size;
    tab[size] = *request;
    while (size > 0 && passes_before(&tab[size], &tab[(size - 1) / 2]))
    {
        tmp = tab[size];
        tab[size] = tab[(size - 1) / 2];
        tab[(size - 1) / 2] = tmp;
        size = (size - 1) / 2;
    }
    heap->size += 1;
    return (0);
}

int heap_pop(t_heap *heap)
{
    int         i;
    t_request   tmp;
    int     smallest;

    if (heap->size == 0)
        return (1);
    heap->tab[0] = heap->tab[heap->size - 1];
    heap->size -= 1;
    i = 0;
    while (1)
    {
        smallest = i;
        if ((2 * i) + 1 < heap->size
        && passes_before(&heap->tab[(2 * i) + 1], &heap->tab[i]))
	        smallest = (2 * i) + 1;
        if ((2 * i) + 2 < heap->size
        && passes_before(&heap->tab[(2 * i) + 2], &heap->tab[smallest]))
	        smallest = (2 * i) + 2;
        if (smallest == i)
            break ;
        tmp = heap->tab[i];
        heap->tab[i] = heap->tab[smallest];
        heap->tab[smallest] = tmp;
        i = smallest;
    }
    return (0);
}

int heap_peek(t_heap *heap, t_request *out)
{
    if (heap->size == 0)
        return (1);
    *out = heap->tab[0];
    return (0);
}