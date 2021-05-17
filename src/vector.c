/*************************************************************************
* 						PROGRAMMER: YARDEN BEN SHIMON
* 						FILE: list Code
* 						DATE: 02-07-2019 16:19
***************************************************************************/

/*------------------Includes----------------------------------------------*/
#include <stdio.h> /* printf*/
#include <stdlib.h> /* calloc, NULL */
#include "../include/vector.h"
/*------------------Macros------------------------------------------------*/
#define MAGIC_NUM 127364 
#define LAST_ITEM(v) (v)->m_items[(v)->m_nItems - 1]
#define OUT_OF_RANGE(v, i) (i) < 0 || (i) >= (v)->m_nItems
#define IS_RESIZE_SMALL(v)\
 ((v)->m_size - (v)->m_nItems) == (v)->m_blockSize * 2 \
 &&  ((v)->m_size - (v)->m_nItems) >  (v)->m_originalSize
/*------------------Structs Definition------------------------------------*/
struct Vector
{
    void **    m_items;
    size_t  m_originalSize;   /* original allocated space for items)*/
    size_t  m_size;              /* actual allocated space for items) */
    size_t  m_nItems;        /* actual number of items */
    size_t  m_blockSize;   /* the chunk size to be allocated when no space*/
    int m_magicNum;
};
/*------------------Static Functions Declaration--------------------------*/
static VectorResult Resize(Vector * _vector, long _newSize);

/*------------------Public Functions-------------------------------------*/
Vector*  VectorCreate (size_t _initialSize, size_t _extensionBlockSize)
{
    Vector * newVector;
    if (_initialSize == 0 && _extensionBlockSize == 0)
    {
        return NULL;
    }
    newVector = (Vector *) malloc(sizeof(Vector));
    if(newVector)
    {   
        newVector->m_items =  (void **) calloc(sizeof(void *) , _initialSize);
        if(NULL == newVector->m_items)
        {
            free(newVector);
            return NULL;
        }
        newVector->m_nItems = 0;
        newVector->m_originalSize = _initialSize;
        newVector->m_blockSize = _extensionBlockSize;
        newVector->m_size = _initialSize;
        newVector->m_magicNum = MAGIC_NUM;
    }

    return newVector;
}

void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
    size_t i;
    Vector * vec = NULL;
    if(_vector)
   {
        vec = (*_vector);
       if (vec && vec->m_magicNum == MAGIC_NUM)
       {
            if (_elementDestroy)
            {
                for (i = 0; i < vec->m_nItems ; i++)
                {
                    (*_elementDestroy)(vec->m_items[i]);
                }
            }
            free(vec->m_items);
            vec->m_magicNum = 0;
            free(*_vector);
       }
       *_vector = NULL;
   } 
}

VectorResult VectorAppend(Vector* _vector, void* _item)
{
    if(NULL == _vector)
    {
        return VECTOR_INITIALIZED_ERROR;
    }
    if(NULL == _item)
    {
        return VECTOR_INITIALIZED_ERROR;/*change */
    }
    if (_vector->m_nItems == _vector->m_size)
    {
        if(_vector->m_blockSize == 0 || Resize(_vector, _vector->m_blockSize) == VECTOR_ALLOCATION_ERROR)
        {
            return VECTOR_ALLOCATION_ERROR;
        }
    }
    _vector->m_items[_vector->m_nItems] = _item;
	(_vector->m_nItems)++;

	return VECTOR_SUCCESS; 
}


VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
    
    if(NULL ==_vector)
    {
        return VECTOR_INITIALIZED_ERROR;
    }
   if (_vector->m_nItems == 0)
   {

       return VECTOR_UNDERFLOW_ERROR;
   }
     if(NULL !=_pValue)
    {
        *_pValue =  LAST_ITEM(_vector);
    }
    --(_vector->m_nItems);
    if (IS_RESIZE_SMALL(_vector))
    {
        Resize(_vector,_vector->m_blockSize  * -1 );
    }
    
   return VECTOR_SUCCESS;
}


VectorResult VectorGet(const Vector* _vector, size_t _index, void** _pValue)
{
    if(NULL ==_pValue)
    {
        return VECTOR_INITIALIZED_ERROR;
    }
    if(NULL ==_vector)
    {
        return VECTOR_INITIALIZED_ERROR;
    }
   if (OUT_OF_RANGE(_vector, _index))
   {

       return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
   }
   *_pValue = _vector->m_items[_index];

   return VECTOR_SUCCESS;
}

VectorResult VectorSet(Vector* _vector, size_t _index, void*  _value)
{
    if(NULL ==_vector)
    {
        return VECTOR_INITIALIZED_ERROR;
    }
    if(NULL ==_value)
    {
        return VECTOR_INITIALIZED_ERROR;
    }

   if (OUT_OF_RANGE(_vector, _index))
   {
       return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
   }
   _vector->m_items[_index] = _value;

   return VECTOR_SUCCESS;
}


size_t VectorSize(const Vector* _vector)
{
    if(NULL ==_vector)
    {
        return 0;
    }
     return  _vector->m_nItems;
}
size_t VectorCapacity(const Vector* _vector)
{
    if(NULL ==_vector)
    {
        return 0;
    }
     return  _vector->m_size;
}

size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
    size_t i = 0;
    if (_vector && _action)
    {  
      for (; i < _vector->m_nItems; i++)
        {
            if((*_action)(_vector->m_items[i],i,_context) == 0)
            {
                i++;
                break;
            }
        }
    }
    
    return i;
}

/*------------------Static Functions Definition----------------------------*/
static VectorResult Resize(Vector * _vector, long _newSize)
{
    void ** tempArr;
    tempArr = (void **) realloc(_vector->m_items, sizeof(void *)*(_vector->m_size + _newSize));
    if (tempArr)
    {
        _vector->m_items = tempArr;
        _vector->m_size += _newSize;
        return VECTOR_SUCCESS;
    }
    return VECTOR_ALLOCATION_ERROR;
}
