// Copyright (c) 2017 Leander Perera
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include "common_utils.h"
#include <cstdlib>
#include <utility>

//! \brief       The dynarray class manages an automatically re-sizing
//!              collection of elements.
template <class T> class dynarray
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
    T *mp_array;           // The backing array that stores the data
    size_t m_capacity;     // Capacity of the backing array
    size_t m_logical_size; // The logical size of the dynamic array
    // The logical size changes when elements are inserted/deleted
};

//! \brief    Construct a dynamic array with initial size.
//!
//! \param[in] size - Amount of space to allocate for the dynamic array.
//!            By default a 0 size array is created if no arguments
//!            are specified.
//!
//! \throws    bad_alloc exception if allocation fails
template <class T> dynarray<T>::dynarray(size_t size)
{
    // Initialize array elements to 0s. The runtime will throw a bad_alloc
    // exception if the allocation fails. The caller is expected to handle
    // the exception.
    mp_array = new T[size]{};

    // bad_alloc exception would have been thrown at this point if allocation
    // failed
    m_capacity = size;
    m_logical_size = size;
}

//! \brief    Deallocates memory allocated for the dynamic array
template <class T> dynarray<T>::~dynarray()
{
    delete[] mp_array;
    mp_array = NULL;
}

//! \brief Returns the element at the specified index
//!
//! \param[in] index - A zero-based index ranging from from 0 to the size
//!            of the array - 1.
template <class T> T dynarray<T>::get_element(size_t index)
{
    T retVal;
    if (mp_array && index < m_logical_size)
    {
        retVal = mp_array[index];
    }

    return retVal;
}

//! \brief       Sets the element at the specified index.
//! \details     The index needs to be within the capacity of the array.
//!              If not the function will return a failure code.
//! \param[in]   index - A zero-based index ranging from from 0 to the size
//!              of the array - 1.
//! \param[in]   element - The element to be stored at the specified index
//! \return      true if successful, false otherwise
template <class T> bool dynarray<T>::set_element(size_t index, T element)
{
    bool retval = false;
    if (mp_array && index < m_logical_size)
    {
        // Store the element at the index
        mp_array[index] = element;
        retval = true;
    }

    return retval;
}

//! \brief        Insert the specified element at the specified index.
//! \param[in]    index - A zero-based index ranging from from 0 to the size
//!               of the array. If the index is equal to the size of the
//!               dynamic array, the element is added to the end of the
//!               dynamic array.
//! \param[in]    element - The item to be inserted into the dynamic array.
//! \throws       bad_alloc exception if allocation fails
template <class T> bool dynarray<T>::insert_element(size_t index, T element)
{
    bool retval = false;

    // Note: Index can be 0 if inserting new lements at the beginning
    // Note: Index can be == m_logical_size if inserting elements at the end
    if (index <= m_logical_size)
    {
        // Index is valid
        if (m_logical_size == m_capacity)
        {
            // The logical size of the backing array has reached its capacity.
            // Resize the backing array to 2x its current capacity
            // (or 1 if its current capacity is 0) and copy the old contents to
            // the
            // new array.
            size_t new_capacity = CALCULATE_NEW_CAPACITY(m_capacity);
            T *new_array = new T[new_capacity]{};

            // Allocation of new array failed. Return failure code
            if (new_array == NULL)
            {
                return false;
            }

            // Copy the rest of the items to the new_array
            for (size_t i = 0, j = 0; i < m_capacity; ++i, ++j)
            {
                if (i == index)
                {
                    // Skip the index for the new element. The new element will
                    // be copied to this location.
                    ++j;
                }
                new_array[j] = mp_array[i];
            }

            delete[] mp_array;    // Deallocate old array
            mp_array = new_array; // Assign new array

            mp_array[index] = element; // Store the new element at the index
            m_logical_size += 1;               // Increment the logical size

            m_capacity =
                new_capacity; // Update the capacity of the backing array
        }
        else
        {
            T temp = element; // Store the element in the temporary storage
            m_logical_size += 1;      // Increment the size by 1

            // Move each element to the right
            for (size_t i = index; i < m_logical_size; ++i)
            {
                std::swap(mp_array[i], temp);
            }
        }
        retval = true;
    }

    return retval;
}

//! \brief     Delete the element at the specified index.
//!
//! \param[in] index - A zero-based index ranging from from 0 to the size
//!            of the array - 1.
template <class T> bool dynarray<T>::delete_element(size_t index)
{
    bool retval = false;

    // Ensure that the array is valid and the size and index are within range
    if (mp_array && m_logical_size > 0 && index < m_logical_size)
    {
        mp_array[index] = {0}; // Clear existing element
        m_logical_size -= 1;           // Decrement the size of the backing array by 1

        // Sift all elements past the deleted element to the left
        for (size_t i = index; i < m_logical_size; ++i)
        {
            std::swap(mp_array[i], mp_array[i + 1]);
        }

        retval = true;
    }
    return retval;
}

//! \brief      Returns the capacity of the backing array
//! \return     capacity of backing array
template <class T> size_t dynarray<T>::array_capacity()
{
    return m_capacity;
}

//! \brief      Returns the logical size of the backing array
//! \return     logical size of the backing array
template <class T> size_t dynarray<T>::array_size()
{
    return m_logical_size;
}

//! \brief      Returns whether or not the array is empty
//! \return     TRUE - array is empty, FALSE otherwise
template <class T> bool dynarray<T>::is_empty()
{
    return (m_logical_size == 0);
}
