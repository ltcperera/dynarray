#pragma once
#include <iostream>

using namespace std;

/**
 * Dynamic Array manages an automatically re-sizing array of elements.
 */
template <class T>
class dynarray
{
public:
    dynarray(unsigned int size);
    ~dynarray();
    T element_at(unsigned int index);
    bool insert_element(unsigned int index, T element);
    bool delete_element(unsigned int index);
    unsigned int array_size();
    bool is_empty();
private:
    unsigned int resize_array(unsigned int newsize);
    T *mp_array;
    unsigned int m_array_size;	// Size allocated for the array
    unsigned int m_last_index;	// The last index of the array
};

/**
 * Resize the array by the specified size
 */
template <class T>
unsigned int dynarray<T>::resize_array(unsigned int newsize) {

}

/**
 * Constructor with initial size
 */
template <class T>
dynarray<T>::dynarray(unsigned int size)
{
    mp_array = new (std::nothrow) T[size]{}; // Initialize array elements to 0s
    m_array_size = 0;
    m_last_index = 0;

    // Set the size if the allocation is successful
    if (mp_array) {
        m_array_size = size;
    }
}

/**
 * Destructor
 */
template <class T>
dynarray<T>::~dynarray()
{
    delete[] mp_array;
    mp_array = NULL;
}

/**
 * Returns the element at the specified index
 * The index is 0 based, so the index parameter is a value
 * from 0 to the size of the array - 1.
 */
template <class T>
T dynarray<T>::element_at(unsigned int index)
{
	T retVal = 0;
	if (mp_array && index <= m_last_index) {
		retVal = mp_array[index];
	}

    return retVal;
}

/**
 * Insert the specified element at the specified index.
 * The index is 0 based, so the index parameter is a value
 * from 0 to the size of the array - 1.
 */
template <class T>
bool dynarray<T>::insert_element(unsigned int index, T element)
{
	// if index >= m_array_size, resize the array
	// if index > m_last_index, m_last_index = index
	// mp_array[index] = element

	return false;
}

/**
 * Delete the element at the specified index.
 * The index is 0 based, so the index parameter is a value
 * from 0 to the size of the array - 1.
 */
template <class T>
bool dynarray<T>::delete_element(unsigned int index)
{
    return false;
}

/**
 * Returns the size of the array
 */
template <class T>
unsigned int dynarray<T>::array_size()
{
    return m_array_size;
}

/**
 * Returns whether or not the array is empty
 *   TRUE - array is empty
 *   FALSE - array is not empty
 */
template <class T>
bool dynarray<T>::is_empty()
{
    return (m_array_size == 0);
}
