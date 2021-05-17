#ifndef __HEAP_H__
#define __HEAP_H__
#include "vector.h"

/** 
 * @brief Create a Binary heap generic data type over a generic vector data type.
 * @author Author Yarden Ben Shimon 
 * @see https://en.wikipedia.org/wiki/Binary_heap 
 */ 


typedef enum HeapResultCode {
	HEAP_SUCCESS,
	HEAP_NOT_INITIALIZED,
	HEAP_REALLOCATION_FAILED,
	HEAP_VALUE_IS_NULL
} HeapResultCode;

typedef struct Heap Heap;

typedef int	(*ActionFunction)(const void *_elem, size_t _index, void * _context);
typedef int	(*LessThanComparator)(const void *_left, const void *_right);

/**  
 * @brief Apply a heap policy over a Vector container.  
 * @param[in] _vector - Vector that hold the elements, must be initialized
 * @param[in] _pfLess - A less than comparator to be used to compare elements 
 * @return Heap * - on success
 * @retval NULL on fail 
 *
 * @warning allocating and freeing the underlying vector is user responsibility. 
 * as long as vector is under the heap control, user should not access the vector directly
 */
Heap* HeapBuild(Vector* _vector, LessThanComparator _pfLess);

/**  
 * @brief Deallocate a previously allocated heap
 * Frees the heap but not the underlying vector  
 * @param[in] _heap - Heap to be deallocated.  On success will be nulled.
 * @return Underlying vector or NULL if heap wasn't initialized or destroyed
 *        
 */
Vector* HeapDestroy(Heap** _heap);

/**  
 * @brief Add an element to the Heap preserving heap property.  
 * @param[in] _heap - Heap to insert element to.
 * @param[in] _element - Element to insert. can't be null
 * @return success or error code 
 * @retval HEAP_OK  on success
 * @retval HEAP_NOT_INITIALIZED  error, heap not initialized or been destroyed
 * @retval HEAP_REALLOCATION_FAILED error, heap could not reallocate underlying vector 
 * @retval HEAP_VALUE_IS_NULL if Element is null
 */
HeapResultCode HeapInsert(Heap* _heap, void* _element);

/**  
 * @brief Peek at element on top of heap without extracting it.  
 * @param[in] _heap - Heap to peek at.
 * @return pointer to element, can be null if heap is empty or not initalized
 */
const void* HeapPeek(const Heap* _heap);

/**  
 * @brief Extract element on top of heap and remove it.  
 * @param[in] _heap - Heap to extract from.
 * @return pointer to element, can be null if heap is empty or not initalized. 
 */
void* HeapExtract(Heap* _heap);

/**  
 * @brief Get the current size (number of elements) in the heap.  
 * @param[in] _heap - the Heap.
 * @return number of elements or zero if empty or not initalized or magicNum diffrent fro, MAGIC_NUM. 
 */
size_t HeapSize(const Heap* _heap);

/**  
 * @brief Iterate over all elements  in the heap from top to bottom.
 * @details The user provided ActionFunction _act will be called for each element. 
 * iteration will stop at the first element where _act(e) returns zero
 * @param[in] _heap - Heap to iterate over.
 * @param[in] _act - User provided function pointer to be invoked for each element
 * @returns number of times the user functions was invoked
 */
size_t HeapForEach(const Heap* _heap, ActionFunction _act, void* _context);


/**  
 * @brief Sort a given vector of elements using a heap sort.
 * @param[in] _vector - vector to sort.
 * @param[in] _pfLess pointer to function to compere
 */
void HeapSort(Vector* _vec, LessThanComparator _pfLess);

#endif /*__HEAP_H__*/
