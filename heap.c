#include "header.h"

int init_heap(t_heap *heap, int capacity)
{
    heap->tab = malloc(sizeof(t_request) * capacity);
    if (!heap->tab)
        return 1;
    heap->size = 0;
    heap->capacity = capacity;
    return 0;
}

int insertion(t_heap *heap, t_request *request)
{
    t_request tmp;
    int size;
    t_request *tab;

    if (heap->size == heap->capacity)
        return 1;
    tab = heap->tab;
    size = heap->size;
    tab[size] = *request;
    while (size > 0 && tab[size].key < tab[(size - 1) / 2].key)
    {
        tmp = tab[size];
        tab[size] = tab[(size - 1) / 2];
        tab[(size - 1) / 2] = tmp;
        size = (size - 1) / 2;
    }
    heap->size += 1;
    return 0;
}

int extract_min(t_heap *heap, t_request *out)
{
    int i;
    t_request tmp;
    int smallest;

    if (heap->size == 0)
        return 1;
    *out = heap->tab[0];
    heap->tab[0] = heap->tab[heap->size - 1];
    heap->size -= 1;
    i = 0;
    while (1)
    {
        smallest = i;
        if ((2 * i) + 1 < heap->size && heap->tab[(2 * i) + 1].key < heap->tab[i].key)
        {
            smallest = (2 * i) + 1;
        }
        if ((2 * i) + 2 < heap->size && heap->tab[(2 * i) + 2].key < heap->tab[smallest].key)
        {
            smallest = (2 * i) + 2;
        }
        if (smallest == i)
            break ;
        tmp = heap->tab[i];
        heap->tab[i] = heap->tab[smallest];
        heap->tab[smallest] = tmp;
        i = smallest;
    }
    return (0);
}

int consultation(t_heap *heap, t_request *out)
{
    if (heap->size == 0)
        return (1);
    *out = heap->tab[0];
    return (0);
}

int main(void)
{
    t_heap heap;
    t_request coder;
    t_request out;

    int i;

    int capacity = 6;
    int id[6] = {1, 2, 3, 4, 5, 6};
    int key[6] = {5, 3, 8, 1, 9, 2};
    init_heap(&heap, capacity);
    i = 0;
    while (i < 6)
    {
        coder.id = id[i];
        coder.key = key[i];
        insertion(&heap, &coder);
        i++;
    }
    i = 0;
    while(i < 6)
    {
        extract_min(&heap, &out);
        printf("%d\n", out.id);
        i++;
    }

}