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
#include "common_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*!
 * \brief       Stores tracking information required to manage the array.
 * \details     Each instance of the dynamic array will have its own instance
 *              of this structure.
 */
typedef struct _DYNARRAY_METADATA
{
  size_t capacity;           /**< The number of elements that can be stored in the backing array */
  size_t element_size;       /**< The size of each element stored in the array */
  size_t logical_size;       /**< The size of the backing array. This is not necessarily the same as the capacity */
  void *p_backing_array;     /**< The backing array that will hold the array data */
} DYNARRAY_METADATA;

/*!
 * \brief       Initialize the dynamic array with the number of elements and
 *              size per element.
 *
 * \param       num_elements - The number of elements to store in the array.
 * \param       element_size - The size of each element in the array.
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
      p_meta_data = NULL;
    }
  }

  return p_meta_data;
}

/*!
 * \brief       Frees the dynamic array identified by the specified handle
 * \param       handle - The handle of the dynamic array to be freed
 */
void free_array(DYNARRAY_HANDLE handle)
{
  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;

    // De-allocate backing array first.
    if (p_meta_data->p_backing_array) {
      free(p_meta_data->p_backing_array);
      p_meta_data->p_backing_array = NULL;
    }

    // Free the metadata structure
    free(p_meta_data);
    p_meta_data = NULL;
  }
}

/*!
 * \brief         Stores the specified data element at the location indicated by
 *                the index.
 * \details       The index needs to be within the capacity. Any previously
 *                stored data at the location is replaced by the new content.
 * \param[in]     handle - The handle of the dynamic array
 * \param[in]     index - The position to store the data element.
 * \param[in]     p_data - The data to be stored.
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

/*!
 * \brief         Gets the element at the specified index.
 * \details       This function returns the element at the specified index from
 *                the dynamic array identified by the specified handle.
 * \param[in]     handle - The handle of the dynamic array
 * \param[in]     index - position of the array element to be returned
 * \param[in,out] p_data - pointer to data element containing data to be returned
 */
bool get_element(DYNARRAY_HANDLE handle, size_t index, void *p_data)
{
  bool status = false;
  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;

    // Ensure array index is within capacity and backing array is valid
    if (p_meta_data->p_backing_array && index < p_meta_data->capacity) {
      void *element_address = p_meta_data->p_backing_array + (p_meta_data->element_size * index);
      memcpy(p_data, element_address, p_meta_data->element_size);
      status = true;
    }
  }

  return status;
}

/*!
 * \brief         Copy elements from the source array to the destination array.
 * \details       Copies elements from the specified index on the source array to
 *                the destination array at the specified index. The destination
 *                array needs to be able to hold the specified number of data
 *                elements of the specified size.
 * \param[in]     p_source_data - Pointer to the source array.
 * \param[in]     source_index - The starting index of the source array.
 *                The num_elements parameter specifies the number of elements
 *                to be copied.
 * \param[in,out] p_dest_data - Pointer to the destination array.
 * \param[in]     dest_index - The starting index of the destination array.
 *                The num_elements parameter specifies the number of elements
 *                to be copied. The destination array needs to be able to hold
 *                the number of elements.
 * \param[in]     data_size - Specifies the size of each data item in the array.
 * \param[in]     num_elements - Specifies the number of elements to be copied.
 */
void copy_elements(void *p_source_data, size_t source_index,
                   void *p_dest_data, size_t dest_index,
                   size_t data_size, size_t num_elements)
{

}

/*!
 * \brief         Insert an element into the array specified by the handle and
 *                index.
 * \details       If the backing array has reached its capacity, the array will
 *                be resized by twice the current capacity.
 * \param[in]     handle - The handle of the dynamic array to be freed
 * \param[in]     index - The position to store the data element.
 * \param[in]     p_data - The data to be stored.
 */
bool insert_element(DYNARRAY_HANDLE handle, size_t index, void *p_data)
{
  bool status = false;
  if (handle) {
    // Handle is non-zero, get DYNARRAY_METADATA from handle
    DYNARRAY_METADATA *p_meta_data = (DYNARRAY_METADATA*) handle;

    // The index can be 0 if inserting at the beginning of the dynamic array
    // The index can be equal to the size of the backing array if inserting
    //   at the end of the dynamic array
    if (index <= p_meta_data->logical_size) {
      // The logical size of the backing array has reached its size
      //   hence the backing array needs to be resized.
      if (p_meta_data->logical_size == p_meta_data->capacity) {
        // Resize the backing array for new element
        size_t new_capacity = CALCULATE_NEW_CAPACITY(p_meta_data->capacity);
        void *p_new_backing_array = calloc(new_capacity, p_meta_data->element_size);

        if (p_new_backing_array) {
          // Copy the elements before the current index
          // Set the current element that was inserted
          // Copy the elements after the current index
        }
      }
    }
  }
  return status;
}

/*!
 * \brief       Delete the element specified by the handle and index.
 * \param[in]   handle - The handle of the dynamic array to be freed
 * \param[in]   index - The position of the data element to be removed
 */
bool delete_element(DYNARRAY_HANDLE handle, size_t index)
{
  return false;
}

/*!
 * \brief       Returns the capacity of the array identified by the specified
 *              handle.
 * \param[in]   handle - The handle of the dynamic array
 * \return      The capacity of the allocated backing array
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
