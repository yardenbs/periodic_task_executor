
/*************************************************************************
* 						PROGRAMMER: YARDEN BEN SHIMON
* 						FILE: heap Code
* 						DATE: 02-07-2019 16:19
***************************************************************************/

/*------------------Includes----------------------------------------------*/
#include <stdio.h>  /* printf*/
#include <stdlib.h> /* calloc, NULL */
#include "vector.h"
#include "heap.h"
/*------------------Macros------------------------------------------------*/

#define MAGIC_NUM 134939
#define LEFT(i) 2 * (i) + 1
#define RIGHT(i) 2 * (i) + 2
#define PARENT(i) ((i)-1) / 2
#define START_INDEX 0
#define YOUNGEST_PARENT(size) ((size) / 2) - 1
/*------------------Structs Definition------------------------------------*/
struct Heap
{
    int m_magicNum;
    Vector *m_vec;
    size_t m_heapSize; /* actual  size of items in heap) */
    LessThanComparator m_LessThenComparator;
};

/*------------------Static Functions Declaration--------------------------*/
void Swap(Vector *_vec, size_t _a, size_t _b);
void BubbleUp(Heap *a_heap, size_t a_start);
void Heapify(Heap *a_heap, size_t a_index);
size_t FindMinIndex(Heap *_heap, size_t _first, size_t _second);
/*------------------Public Functions-------------------------------------*/
Heap *HeapBuild(Vector *_vector, LessThanComparator _pfLess)
{
    Heap *newHeap;
    long i;

    if (0 == VectorCapacity(_vector) || NULL == _pfLess)
    {
        return NULL;
    }

    newHeap = (Heap *)malloc(sizeof(Heap));
    if (newHeap)
    {

        newHeap->m_magicNum = MAGIC_NUM;
        newHeap->m_LessThenComparator = _pfLess;
        newHeap->m_vec = _vector;
        newHeap->m_heapSize = VectorSize(_vector);

        for (i = YOUNGEST_PARENT(newHeap->m_heapSize); i >= 0; --i)
        {
            Heapify(newHeap, i);
        }
    }
    return newHeap;
}

Vector *HeapDestroy(Heap **_heap)
{
    Vector *vec = NULL;
    Heap *heap;
    if (_heap)
    {
        heap = *_heap;
        if (heap)
        {
            if (heap->m_magicNum == MAGIC_NUM)
            {
                heap->m_magicNum = 0;
                vec = heap->m_vec;
                free(heap);
            }
            *_heap = NULL;
        }
    }

    return vec;
}
HeapResultCode HeapInsert(Heap *_heap, void *_element)
{
    VectorResult vectorStat;

    if (NULL == _heap || _heap->m_magicNum != MAGIC_NUM)
    {
        return HEAP_NOT_INITIALIZED;
    }
    if (NULL == _element)
    {
        return HEAP_VALUE_IS_NULL;
    }

    vectorStat = VectorAppend(_heap->m_vec, _element);
    if (vectorStat == VECTOR_ALLOCATION_ERROR)
    {
        return HEAP_REALLOCATION_FAILED;
    }

    _heap->m_heapSize++;
    BubbleUp(_heap, _heap->m_heapSize - 1);

    return HEAP_SUCCESS;
}

const void *HeapPeek(const Heap *_heap)
{
    void *top = NULL;
    if (_heap && _heap->m_magicNum == MAGIC_NUM)
    {
        VectorGet(_heap->m_vec, START_INDEX, &top);
    }
    return top;
}

void *HeapExtract(Heap *_heap)
{
    void *lastItem;
    void *top = NULL;

    if (_heap && _heap->m_magicNum == MAGIC_NUM && _heap->m_heapSize != 0)
    {
        if (VECTOR_SUCCESS == VectorGet(_heap->m_vec, START_INDEX, &top))
        {
            VectorGet(_heap->m_vec, _heap->m_heapSize - 1, &lastItem);
            VectorSet(_heap->m_vec, START_INDEX, lastItem);
        }

        VectorRemove(_heap->m_vec, NULL);
        _heap->m_heapSize--;
        Heapify(_heap, START_INDEX);
    }

    return top;
}

size_t HeapSize(const Heap *_heap)
{
    size_t size = 0;
    if (_heap && _heap->m_magicNum == MAGIC_NUM)
    {
        size = _heap->m_heapSize;
    }

    return size;
}

size_t HeapForEach(const Heap *_heap, ActionFunction _act, void *_context)
{
    size_t callBacksNum = 0;
    if (_heap && _heap->m_magicNum == MAGIC_NUM && _act)
    {
        callBacksNum = VectorForEach(_heap->m_vec, (VectorElementAction)_act, _context);
    }
    return callBacksNum;
}

void HeapSort(Vector *_vec, LessThanComparator _pfLess)
{
    Heap *heap = HeapBuild(_vec, _pfLess);
    if (heap)
    {
        while (heap->m_heapSize != 0)
        {
            Swap(heap->m_vec, START_INDEX, heap->m_heapSize - 1);
            heap->m_heapSize--;
            Heapify(heap, START_INDEX);
        }
    }
    free(heap);
}

/*------------------Static Functions Definition----------------------------*/
void Swap(Vector *_vec, size_t _a, size_t _b)
{
    void *left, *right;

    VectorGet(_vec, _a, &left);
    VectorGet(_vec, _b, &right);
    VectorSet(_vec, _a, right);
    VectorSet(_vec, _b, left);
}

void Heapify(Heap *a_heap, size_t a_index)
{
    long left, right, largest = a_index;

    while (a_index < a_heap->m_heapSize - 1)
    {
        left = LEFT(a_index);
        right = RIGHT(a_index);

        if (a_heap->m_heapSize > left)
        {
            largest = FindMinIndex(a_heap, left, largest);
        }
        if (a_heap->m_heapSize > right)
        {
            largest = FindMinIndex(a_heap, right, largest);
        }
        if (largest == a_index)
        {
            break;
        }

        Swap(a_heap->m_vec, largest, a_index);
        a_index = largest;
    }
}

size_t FindMinIndex(Heap *_heap, size_t _first, size_t _second)
{
    void *firstElem, *secondElem;
    VectorGet(_heap->m_vec, _first, &firstElem);
    VectorGet(_heap->m_vec, _second, &secondElem);
    return _heap->m_LessThenComparator(firstElem, secondElem) > 0 ? _second : _first;
}
void BubbleUp(Heap *a_heap, size_t a_start)
{
    long largest, parent;
    while (a_start != START_INDEX)
    {
        parent = PARENT(a_start);
        if (parent < START_INDEX)
        {
            break;
        }

        largest = FindMinIndex(a_heap, a_start, parent);
        if (largest == parent)
        {
            break;
        }
        Swap(a_heap->m_vec, largest, parent);
        a_start = parent;
    }
}
