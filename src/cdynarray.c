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
#include <string.h>

/**
 * Stores tracking information required to manage the array.
 * Each instance of the dynamic array will have its own instance of this
 *   structure.
 */
typedef struct _DYNARRAY_METADATA
{
  size_t capacity;           /**< The number of elements that can be stored in the backing array */
  size_t element_size;       /**< The size of each element stored in the array */
  void *p_backing_array;     /**< The backing array that will hold the array data */
} DYNARRAY_METADATA;

/**
 * Initialize the dynamic array with the number of elements and size per element.
 *
 * @param num_elements The number of elements to store in the array.
 * @param element_size The size of each element in the array.
 */
DYNARRAY_HANDLE init_array(size_t num_elements, size_t element_size)
{
  /* Allocate metadata structure first. This structure will store information
     about the dynamic array. */

  DYNARRAY_METADATA *p_meta_data = malloc(sizeof(DYNARRAY_METADATA));

  if (p_meta_data) {
    /* Initialize metadata structure to known initialization defaults */
    p_meta_data->capacity = 0;

    /* Meta data structure successfully allocated. Allocate backing array
         that will store the elements of the dynamic array */
    void *p_backing_array = calloc(num_elements, element_size);

    if (p_backing_array) {
      /* Backing array was successfully allocated.
           Set metadata members accordingly. */
      p_meta_data->p_backing_array = p_backing_array;

      /* Capacity and element size are set accordingly */
      p_meta_data->capacity = num_elements;
      p_meta_data->element_size = element_size;
    }
    else {
      /* Allocation of backing array failed. De-allocate metadata structure
           as well since there is no longer a use for the metadata structure. */
      free(p_meta_data);
    }
  }

  return p_meta_data;
}

/**
 * Frees the dynamic array identified by the specified handle
 *
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
    }

    // Free the metadata structure
    free(p_meta_data);
  }
}

/**
 * Stores the specified data element at the location indicated by the index.
 * The index needs to be within the capacity. Any previously stored data at the
 * location is replaced by the new content.
 *
 * @param handle The handle of the dynamic array to be freed
 * @param index The position to store the data element.
 * @param p_data The data to be stored.
 */
bool set_element(DYNARRAY_HANDLE handle, size_t index, void *p_data)
{
  bool status = false;
  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;

    // Ensure array index is within capacity and backing array is valid
    if (p_meta_data->p_backing_array && index < p_meta_data->capacity) {
      // Copy the data into the backing array. The size is determined by the element_size
      //   that was passed in when the dynamic array was allocated.
      // Element address is calculated as the base + the size of the data item * index
      void *element_address = p_meta_data->p_backing_array + (p_meta_data->element_size * index);
      memcpy(element_address, p_data, p_meta_data->element_size);
      status = true;
    }
  }

  return status;
}

/**
 * Insert an element into the array specified by the handle and index.
 *
 * If the backing array has reached its capacity, the array will be resized by
 * twice the current capacity.
 *
 * @param handle The handle of the dynamic array to be freed
 * @param index The position to store the data element.
 * @param p_data The data to be stored.
 */
bool insert_element(DYNARRAY_HANDLE handle, size_t index, void *p_data)
{
  return false;
}

/**
 * Delete the element specified by the handle and index.
 *
 * @param handle The handle of the dynamic array to be freed
 * @param index The position of the data element to be removed
 */
bool delete_element(DYNARRAY_HANDLE handle, size_t index)
{
  return false;
}

/**
 * Returns the capacity of the array identified by the specified handle
 *
 * @param handle The handle of the dynamic array
 * @return the capacity of the allocated backing array
 */
size_t array_capacity(DYNARRAY_HANDLE handle)
{
  size_t capacity = 0;

  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;
    capacity = p_meta_data->capacity;
  }

  return capacity;
}
