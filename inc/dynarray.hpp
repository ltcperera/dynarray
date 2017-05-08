// Copyright (c) 2017 Leander Perera
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <cstdlib>

/// The dynarray class manages an automatically re-sizing collection of elements.
template <class T>
class dynarray
{
public:
  dynarray(size_t size = 0);
  ~dynarray();
  T get_element(size_t index);
  bool set_element(size_t index, T element);
  bool insert_element(size_t index, T element);
  bool delete_element(size_t index);
  size_t array_capacity();
  size_t array_size();
  bool is_empty();
private:
  bool resize_array(size_t newsize);
  T *mp_array;          // The backing array that stores the data
  size_t m_capacity;	  // Capacity of the backing array
  size_t m_size;        // The logical size of the dynamic array
                        // The logical size changes when elements are inserted/deleted
};

/// Function Swaps the values of the first and second arguments passed
///
/// @param[in,out] first reference to first item to swap
/// @param[in,out] second reference to second item to swap
template <class T>
void swap(T &first, T &second)
{
  T temp = first;
  first = second;
  second = temp;
}

/// Function that copies the specified range of elements from the source array to the destination array
///
/// @param[in,out] dest_array The destination array to which elements will be copied to
/// @param[in] src_array The source array to which elements will be copied from
/// @param[in] from_index The starting index for the copy
/// @param[in] to_index The ending index for the copy
template <class T>
bool copy_array_range(T &dest_array, const T &src_array, size_t from_index, size_t to_index)
{
  return false;
}

/// Resize the array by the specified size.
///
/// The function creates a new array of the specified size and copies the
/// contents from the old array to the new array. The old array is deallocated.
/// The function also updates the size of the dynamic array to the new size.
/// The newsize needs to be larger than the currently allocated size of the
/// array.
///
/// @param[in] newsize the size of the reallocated array
/// @return    true if successful, false otherwise
template <class T>
bool dynarray<T>::resize_array(size_t newsize)
{
  bool retval = false;

  // Only resize if new size is larger than the currently allocated capacity
  if (newsize > m_capacity) {
    // Allocate new array of specified capacity
    T *p_new_array = new T[newsize]{}; // Initialize the elements to 0s
    if (p_new_array) {
      // Copy over elements from old array to new array
      for (unsigned int i = 0; i < m_capacity; ++i) {
        p_new_array[i] = mp_array[i];
      }

      // De-allocate the old array and make it point to the new array
      delete []mp_array;
      mp_array = p_new_array;

      // Update the new size of the array
      m_capacity = newsize;
      retval = true;
    }
  }

  return retval;
}

/// Constructor with initial size for the dynamic array.
///
/// @param[in] size Amount of space to allocate for the dynamic array.
///                 By default a 0 size array is created if no arguments
///                 are specified.
template <class T>
dynarray<T>::dynarray(size_t size)
{
  // Initialize array elements to 0s. Ensure that allocation
  //  does not throw an exception, but rather returns a NULL on failure.
  mp_array = new (std::nothrow) T[size]{};
  m_capacity = 0;
  m_size = 0;

  // Set the size if the allocation was successful
  if (mp_array) {
    m_capacity = size;
    m_size = size;
  }
}

/// Destructor deallocates memory allocated for the dynamic array
template <class T>
dynarray<T>::~dynarray()
{
  delete[] mp_array;
  mp_array = NULL;
}

/// Returns the element at the specified index
///
/// @param[in] index A zero-based index ranging from from 0 to the size
//                   of the array - 1.
template <class T>
T dynarray<T>::get_element(size_t index)
{
	T retVal;
	if (mp_array && index < m_size) {
		retVal = mp_array[index];
	}

  return retVal;
}

/// Sets the element at the specified index.
///
/// The index needs to be within the capacity of the array. If not the function
/// will return a failure code.
///
/// @param[in] index A zero-based index ranging from from 0 to the size
///                  of the array - 1.
/// @param[in] element The element to be stored at the specified index
/// @return true if successful, false otherwise
template <class T>
bool dynarray<T>::set_element(size_t index, T element)
{
  bool retval = false;
  if (mp_array && index < m_size) {
    // Store the element at the index
    mp_array[index] = element;
    retval = true;
  }

  return retval;
}

/// Insert the specified element at the specified index.
///
/// @param[in] index A zero-based index ranging from from 0 to the size
///                  of the array. If the index is equal to the size of the
///                  dynamic array, the element is added to the end of the
///                  dynamic array.
/// @param[in] element The item to be inserted into the dynamic array.
template <class T>
bool dynarray<T>::insert_element(size_t index, T element)
{
  // Note: Index can be 0 if inserting new lements at the beginning
  // Note: Index can be == m_size if inserting elements at the end
  if (index <= m_size) {
    // Index is valid
    if (m_size == m_capacity) {
      // The logical size of the backing array has reached its capacity.
      // Resize the backing array to 2x its current capacity
      // (or 1 if its current capacity is 0) and copy the old contents to the
      // new array.
      size_t new_capacity = (m_capacity == 0) ? 1 :2 * m_capacity;
      T *new_array = new T[new_capacity]{};

      // Allocation of new array failed. Return failure code
      if (new_array == NULL) {
        return false;
      }

      // Copy the rest of the items to the new_array
      for (size_t i = 0, j = 0; i < m_capacity; ++i, ++j) {
        if (i == index) {
          // Skip the index for the new element. The new element will be copied to this location.
          ++j;
        }
        new_array[j] = mp_array[i];
      }

      delete[] mp_array;         // Deallocate old array
      mp_array = new_array;      // Assign new array

      mp_array[index] = element; // Store the new element at the index
      m_size += 1;               // Increment the logical size

      m_capacity = new_capacity; // Update the capacity of the backing array
    }
    else {
      T temp = element; // Store the element in the temporary storage
      m_size += 1;      // Increment the size by 1

      // Move each element to the right
      for (size_t i = index; i < m_size; ++i) {
        swap(mp_array[i], temp);
      }
    }
  }

	return true;
}

/// Delete the element at the specified index.
///
/// @param[in] index A zero-based index ranging from from 0 to the size
///                  of the array - 1.
template <class T>
bool dynarray<T>::delete_element(size_t index)
{
  bool retval = false;

  // Ensure that size and index is within range
  if (m_size > 0 && index < m_size) {
    mp_array[index] = {0};  // Clear existing element
    m_size -= 1;            // Decrement the size of the backing array by 1

    // Sift all elements past the deleted element to the left
    for (size_t i = index; i < m_size; ++i) {
      swap(mp_array[i], mp_array[i + 1]);
    }

    retval = true;
  }
  return retval;
}

/// Returns the capacity of the backing array
/// @return capacity of backing array
template <class T>
size_t dynarray<T>::array_capacity()
{
  return m_capacity;
}

/// Returns the logical size of the backing array
/// @return logical size of the backing array
template <class T>
size_t dynarray<T>::array_size()
{
  return m_size;
}

/// Returns whether or not the array is empty
/// @return TRUE - array is empty, FALSE otherwise
template <class T>
bool dynarray<T>::is_empty()
{
  return (m_size == 0);
}
