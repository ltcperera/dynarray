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
#ifndef __CDYNARRAY_H
#define __CDYNARRAY_H
#include <stdlib.h>
#include <stdbool.h>

typedef void* DYNARRAY_HANDLE; /* Handle to array metadata passed around */

/* Initialize a dynamic array and return a handle to the array created */
DYNARRAY_HANDLE init_array(size_t num_elements, size_t size_of_element);

/* Free the dynamic array specified by the handle */
void free_array(DYNARRAY_HANDLE handle);

/* Insert the specified data item into the dynamic array at the specified index */
bool set_element(DYNARRAY_HANDLE handle, unsigned int index, void *p_data);

/* Insert the specified data item into the dynamic array at the specified index */
bool insert_element(DYNARRAY_HANDLE handle, unsigned int index, void *p_data);

/* Delete the element at the specified index */
bool delete_element(DYNARRAY_HANDLE handle, unsigned int index);

/* Get the capacity of the backing array */
unsigned int array_capacity(DYNARRAY_HANDLE handle);

#endif
