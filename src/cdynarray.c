/*
   Copyright (c) 2017 Leander Perera

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include "cdynarray.h"
#include <stdio.h>
typedef struct _DYNARRAY_METADATA
{
  unsigned int capacity;
  unsigned int last_index;
  void *p_backing_array; /* The backing array that will hold the array data */
} DYNARRAY_METADATA;

/*
 * Initialize the dynamic array with the number of elements and size per element
 *
 * @param num_elements The number of elements to store in the array.
 * @param size_of_element The size of each element in the array.
 */
DYNARRAY_HANDLE init_array(size_t num_elements, size_t size_of_element)
{
  /* Allocate metadata structure first. This structure will store information
     about the dynamic array. */

  DYNARRAY_METADATA *p_meta_data = malloc(sizeof(DYNARRAY_METADATA));

  if (p_meta_data) {
    /* Initialize metadata structure to known initialization defaults */
    p_meta_data->capacity = 0;
    p_meta_data->last_index = 0;

    /* Meta data structure successfully allocated. Allocate backing array
         that will store the elements of the dynamic array */
    void *p_backing_array = calloc(num_elements, size_of_element);

    if (p_backing_array) {
      /* Backing array was successfully allocated.
           Set metadata members accordingly. */
      p_meta_data->p_backing_array = p_backing_array;

      /* Capacity is set to the number of elements allocated */
      p_meta_data->capacity = num_elements;

      /* We haven't added elements yet. Set the last_index to 0. */
      p_meta_data->last_index = 0;
    }
    else {
      /* Allocation of backing array failed. De-allocate metadata structure
           as well since there is no longer a use for the metadata structure. */
      free(p_meta_data);
    }
  }

  return p_meta_data;
}

/*
 * Frees the dynamic array identified by the specified handle
 * @param handle The handle of the dynamic array to be freed
 */
void free_array(DYNARRAY_HANDLE handle)
{
  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;

    // De-allocate backing array first.
    if (p_meta_data->p_backing_array) {
      free(p_meta_data->p_backing_array);
      // Now free the metadata structure
      free(p_meta_data);
    }
  }
}

/*
 * Returns the capacity of the array identified by the specified handle
 * @param handle The handle of the dynamic array
 * @return the capacity of the allocated backing array
 */
unsigned int array_capacity(DYNARRAY_HANDLE handle)
{
  unsigned int capacity = 0;

  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;
    capacity = p_meta_data->capacity;
  }

  return capacity;
}
