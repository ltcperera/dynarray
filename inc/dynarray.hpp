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

/// The dynarray class manages an automatically re-sizing collection of elements.
template <class T>
class dynarray
{
public:
  dynarray(unsigned int size = 0);
  ~dynarray();
  T element_at(unsigned int index);
  bool insert_element(unsigned int index, T element);
  bool delete_element(unsigned int index);
  unsigned int array_capacity();
  bool is_empty();
private:
  bool resize_array(int newsize);
  T *mp_array;
  unsigned int m_capacity;	  // Size allocated for the array
  unsigned int m_last_index;	// The last index of the array
};

/// Function Swaps the values of the first and second arguments passed
///
/// @param[in,out] reference to first item to swap
/// @param[in,out] reference to second item to swap
template <class T>
void swap(T &first, T &second)
{
  T temp = first;
  first = second;
  second = temp;
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
bool dynarray<T>::resize_array(int newsize)
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
dynarray<T>::dynarray(unsigned int size)
{
  // At least allocate an array of size 0
  if (size < 0) {
    size = 0;
  }

  // Initialize array elements to 0s. Ensure that allocation
  //  does not throw an exception, but rather returns a NULL on failure.
  mp_array = new (std::nothrow) T[size]{};
  m_capacity = 0;
  m_last_index = 0;

  // Set the size if the allocation is successful
  if (mp_array) {
      m_capacity = size;
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
T dynarray<T>::element_at(unsigned int index)
{
	T retVal = 0;
	if (mp_array && index <= m_last_index) {
		retVal = mp_array[index];
	}

  return retVal;
}

/// Insert the specified element at the specified index.
///
/// @param[in] index A zero-based index ranging from from 0 to the size
///                  of the array - 1.
template <class T>
bool dynarray<T>::insert_element(unsigned int index, T element)
{
  bool retval = true; // Assume successful
  // if index >= m_capacity, resize the array
  if (index >= m_capacity) {
    // The new capacity ranges from 1 to 2 * index
    int newsize = (m_capacity == 0) ? 1 : index * 2;
    retval = resize_array(newsize);
  }

  if (retval == true) {
	  // Advance the last array index
    if (index > m_last_index) {
      m_last_index = index;
    }

    // Store the element in the specified location
	  mp_array[index] = element;
  }

	return retval;
}


/// Delete the element at the specified index.
///
/// @param[in] index A zero-based index ranging from from 0 to the size
///                  of the array - 1.
template <class T>
bool dynarray<T>::delete_element(unsigned int index)
{
    return false;
}

/// Returns the capacity of the backing array
/// @return capacity of backing array
template <class T>
unsigned int dynarray<T>::array_capacity()
{
    return m_capacity;
}

/// Returns whether or not the array is empty
/// @return TRUE - array is empty, FALSE otherwise
template <class T>
bool dynarray<T>::is_empty()
{
    return (m_capacity == 0);
}
