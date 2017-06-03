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
#include "dynarray.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <string>
using namespace std;

// Prevent C++ compiler from mangling C names
//   so that linker is able to find the functions
extern "C" {
#include "cdynarray.h"
}

/// Structure used to test use of abstract data types with array list
typedef struct
{
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
} BOOK;

/// CPPInterface: Tests default constructor that constructs a 0 length array
TEST(CPPInterface, ConstructionZeroLength)
{
    dynarray<int> arr;
    EXPECT_EQ(arr.array_size(), 0);
    EXPECT_EQ(arr.array_capacity(), 0);
}

/// CPPInterface: Tests constructor with specified array length
TEST(CPPInterface, ConstructionSpesifiedLength)
{
    dynarray<int> arr(20);
    EXPECT_EQ(arr.array_capacity(), 20);
    EXPECT_EQ(arr.array_size(), 20);
}

/// CPPInterface: Tests insertion of elements with resize operation
TEST(CPPInterface, ResizeOperationInsertFirst)
{
    dynarray<int> arr;

    // Insert element at 0th index
    EXPECT_EQ(arr.insert_element(0, 1), true);

    // Expect the capacity and size to resize to 1
    EXPECT_EQ(arr.array_capacity(), 1);
    EXPECT_EQ(arr.array_size(), 1);

    // Insert element at 0th index
    EXPECT_EQ(arr.insert_element(0, 2), true);

    // Expect the capacity and size to resize to 2
    EXPECT_EQ(arr.array_capacity(), 2);
    EXPECT_EQ(arr.array_size(), 2);

    // Insert element at 0th index
    EXPECT_EQ(arr.insert_element(0, 3), true);

    // Expect capacity to resize to 4 and size to be incremented
    EXPECT_EQ(arr.array_capacity(), 4);
    EXPECT_EQ(arr.array_size(), 3);
}

/// CPPInterface: Tests setting of elements at beginning, middle and end
TEST(CPPInterface, SetGetOperations)
{
    // Allocate array for 5 elements (indexed 0 - 4)
    dynarray<int> arr(5);

    // Verify set operations
    EXPECT_EQ(arr.set_element(0, 1), true);
    EXPECT_EQ(arr.set_element(1, 2), true);
    EXPECT_EQ(arr.set_element(2, 3), true);
    EXPECT_EQ(arr.set_element(3, 4), true);
    EXPECT_EQ(arr.set_element(4, 5), true);

    // Verify get operations
    EXPECT_EQ(arr.get_element(0), 1);
    EXPECT_EQ(arr.get_element(1), 2);
    EXPECT_EQ(arr.get_element(2), 3);
    EXPECT_EQ(arr.get_element(3), 4);
    EXPECT_EQ(arr.get_element(4), 5);
}

/// CPPInterface: Tests insertion of elements at beginning, middle and end
TEST(CPPInterface, InsertOperations)
{
    // Allocate array for 5 elements (indexed 0 - 4)
    dynarray<int> arr(5);

    // Setup by storing elements
    arr.set_element(0, 1);
    arr.set_element(1, 2);
    arr.set_element(2, 3);
    arr.set_element(3, 4);
    arr.set_element(4, 5);

    // Verify insert operations
    EXPECT_EQ(arr.insert_element(0, 0xfa), true); // Insert 0xfa at index 0
    EXPECT_EQ(arr.array_size(), 6); // Size should have increased by one

    // Capacity should have doubled from 5 to 10 since
    //    logical size would = capacity
    EXPECT_EQ(arr.array_capacity(), 10);

    EXPECT_EQ(arr.insert_element(3, 0xfb), true); // Insert 0xfb at index 3
    EXPECT_EQ(arr.array_size(), 7);      // Size should have increased by one
    EXPECT_EQ(arr.array_capacity(), 10); // No change in capacity
    EXPECT_EQ(arr.insert_element(7, 0xfc), true); // Insert 0xfc at the end
    EXPECT_EQ(arr.array_size(), 8);      // Size should have increased by one
    EXPECT_EQ(arr.array_capacity(), 10); // No change in capacity

    // Read back all values and verify
    EXPECT_EQ(arr.get_element(0), 0xfa);
    EXPECT_EQ(arr.get_element(1), 1);
    EXPECT_EQ(arr.get_element(2), 2);
    EXPECT_EQ(arr.get_element(3), 0xfb);
    EXPECT_EQ(arr.get_element(4), 3);
    EXPECT_EQ(arr.get_element(5), 4);
    EXPECT_EQ(arr.get_element(6), 5);
    EXPECT_EQ(arr.get_element(7), 0xfc);
}

/// CPPInterface: Tests insertion of elements at beginning,
///    middle and end (string type)
TEST(CPPInterface, InsertOperationsString)
{
    // Allocate array for 5 elements (indexed 0 - 4)
    dynarray<string> arr(5);

    // Setup by storing elements
    arr.set_element(0, "one");
    arr.set_element(1, "two");
    arr.set_element(2, "three");
    arr.set_element(3, "four");
    arr.set_element(4, "five");

    // Verify insert operations
    EXPECT_EQ(arr.insert_element(0, "two hundred and fifty"),
              true);                // Insert 0xfa at index 0
    EXPECT_EQ(arr.array_size(), 6); // Size should have increased by one

    // Capacity should have doubled from 5 to 10 since logical
    //    size would = capacity
    EXPECT_EQ(arr.array_capacity(), 10);

    EXPECT_EQ(arr.insert_element(3, "two hundred and fifty one"),
              true);                     // Insert 0xfb at index 3
    EXPECT_EQ(arr.array_size(), 7);      // Size should have increased by one
    EXPECT_EQ(arr.array_capacity(), 10); // No change in capacity
    EXPECT_EQ(arr.insert_element(7, "two hundred and fifty two"),
              true);                     // Insert 0xfc at the end
    EXPECT_EQ(arr.array_size(), 8);      // Size should have increased by one
    EXPECT_EQ(arr.array_capacity(), 10); // No change in capacity

    // Read back all values and verify
    EXPECT_EQ(arr.get_element(0), "two hundred and fifty");
    EXPECT_EQ(arr.get_element(1), "one");
    EXPECT_EQ(arr.get_element(2), "two");
    EXPECT_EQ(arr.get_element(3), "two hundred and fifty one");
    EXPECT_EQ(arr.get_element(4), "three");
    EXPECT_EQ(arr.get_element(5), "four");
    EXPECT_EQ(arr.get_element(6), "five");
    EXPECT_EQ(arr.get_element(7), "two hundred and fifty two");
}

/// CPPInterface: Tests insertion of values of abstract data types
TEST(CPPInterface, InsertADTValues)
{
    // Build book object that will be stored in the list
    BOOK book;
    book.book_id = 1;
    strcpy(book.title, "The C++ Programming Language");
    strcpy(book.author, "Bjarne Stroustrup");
    strcpy(book.subject, "Programming Languages");

    dynarray<BOOK> arr(1);
    arr.set_element(0, book);

    // Verify if data was stored correctly
    BOOK book2 = arr.get_element(0);
    EXPECT_EQ(book.book_id, book2.book_id);
    EXPECT_EQ(strcmp(book.title, book2.title), 0);
    EXPECT_EQ(strcmp(book.author, book2.author), 0);
    EXPECT_EQ(strcmp(book.subject, book2.subject), 0);
}

/// CPPInterface: Tests the deletion of elements at the beginning,
///    middle and end
TEST(CPPInterface, DeleteElements)
{
    // Allocate the array
    dynarray<int> arr(7);

    arr.set_element(0, 1);
    arr.set_element(1, 2);
    arr.set_element(2, 3);
    arr.set_element(3, 4);
    arr.set_element(4, 5);
    arr.set_element(5, 6);
    arr.set_element(6, 7);

    // Delete elements
    EXPECT_EQ(arr.delete_element(0), true);
    EXPECT_EQ(arr.delete_element(3), true);
    EXPECT_EQ(arr.delete_element(4), true);

    // Verify existing elements
    EXPECT_EQ(arr.get_element(0), 2);
    EXPECT_EQ(arr.get_element(1), 3);
    EXPECT_EQ(arr.get_element(2), 4);
    EXPECT_EQ(arr.get_element(3), 6);

    // Verify the array size
    EXPECT_EQ(arr.array_size(), 4);
}

// ======================
// Tests for C-Interfaces
// ======================

/// CInterface: Test zero sized dynamic array
TEST(CInterface, ConstructionZeroLength)
{
    DYNARRAY_HANDLE handle = init_array(0, sizeof(int));
    EXPECT_EQ(array_size(handle), 0);
    EXPECT_EQ(array_capacity(handle), 0);

    // De-allocate the array
    free_array(handle);
}

/// CInterface: Test construction of a dynamic array using the C interface
///   The test allocates the array and checks its capacity.
TEST(CInterface, ConstructionSpesifiedLength)
{
    // Allocate dynamic array for 10 integers
    DYNARRAY_HANDLE handle = init_array(10, sizeof(int));
    EXPECT_EQ(array_size(handle), 10);
    EXPECT_EQ(array_capacity(handle), 10);

    // De-allocate the array
    free_array(handle);
}

/// CInterface: Test insertion of elements with resize operation
TEST(CInterface, ResizeOperationInsertFirst)
{
    // Start with a 0-sized array
    DYNARRAY_HANDLE handle = init_array(0, sizeof(int));

    // Data that will be inserted
    int a[] = {1, 2, 3};

    // Insert element at 0th index
    EXPECT_EQ(insert_element(handle, 0, &a[0]), true);

    // Expect the capacity and logical size increase to 1
    EXPECT_EQ(array_capacity(handle), 1);
    EXPECT_EQ(array_size(handle), 1);

    // Insert element at 0th index
    EXPECT_EQ(insert_element(handle, 0, &a[1]), true);

    // Expect the array to resize to 2
    EXPECT_EQ(array_capacity(handle), 2);
    EXPECT_EQ(array_size(handle), 2);

    // Insert element at 0th index
    EXPECT_EQ(insert_element(handle, 0, &a[2]), true);

    // Expect the array to resize to 4
    EXPECT_EQ(array_capacity(handle), 4);
    EXPECT_EQ(array_size(handle), 3);

    free_array(handle);
}

/// CInterface: Tests setting of elements at beginning, middle and end
TEST(CInterface, SetGetOperations)
{
    // Allocate array for 5 elements (indexed 0 - 4)
    DYNARRAY_HANDLE handle = init_array(5, sizeof(int));

    // Data that will be stored
    int a[] = {1, 2, 3, 4, 5};
    int b[5];

    // Verify set operations
    EXPECT_EQ(set_element(handle, 0, &a[0]), true);
    EXPECT_EQ(set_element(handle, 1, &a[1]), true);
    EXPECT_EQ(set_element(handle, 2, &a[2]), true);
    EXPECT_EQ(set_element(handle, 3, &a[3]), true);
    EXPECT_EQ(set_element(handle, 4, &a[4]), true);

    // Verify get operations
    EXPECT_EQ(get_element(handle, 0, &b[0]) && b[0] == a[0], true);
    EXPECT_EQ(get_element(handle, 1, &b[1]) && b[1] == a[1], true);
    EXPECT_EQ(get_element(handle, 2, &b[2]) && b[2] == a[2], true);
    EXPECT_EQ(get_element(handle, 3, &b[3]) && b[3] == a[3], true);
    EXPECT_EQ(get_element(handle, 4, &b[4]) && b[4] == a[4], true);

    free_array(handle);
}

/// CInterface: Tests insertion of elements at the beginning, middle and end
TEST(CInterface, InsertOperations)
{
    // Allocate array for 5 elements (indexed 0 - 4)
    DYNARRAY_HANDLE handle = init_array(5, sizeof(int));

    // Data that will be stored
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {0xfa, 0xfb, 0xfc};
    int c[8];

    set_element(handle, 0, &a[0]);
    set_element(handle, 1, &a[1]);
    set_element(handle, 2, &a[2]);
    set_element(handle, 3, &a[3]);
    set_element(handle, 4, &a[4]);

    // Verify insert operations
    EXPECT_EQ(insert_element(handle, 0, &b[0]), true);
    EXPECT_EQ(array_size(handle), 6); // Size should have increased by one

    // Capacity should have doubled from 5 to 10 since
    //    logic size would = capacity
    EXPECT_EQ(array_capacity(handle), 10);

    EXPECT_EQ(insert_element(handle, 3, &b[2]), true);
    EXPECT_EQ(array_size(handle), 7); // Size should have increased by one
    EXPECT_EQ(array_capacity(handle), 10); // No change in capacity
    EXPECT_EQ(insert_element(handle, 7, &b[3]), true);
    EXPECT_EQ(array_size(handle), 8); // Size should have increased by one
    EXPECT_EQ(array_capacity(handle), 10); // No change in capacity

    // Read back all values and verify
    EXPECT_EQ(get_element(handle, 0, &c[0]) && c[0] == b[0], true); // Expected 0xfa
    EXPECT_EQ(get_element(handle, 1, &c[1]) && c[1] == a[0], true); // Expected 1
    EXPECT_EQ(get_element(handle, 2, &c[2]) && c[2] == a[1], true); // Expected 2
    EXPECT_EQ(get_element(handle, 3, &c[3]) && c[3] == b[1], true); // Expected 0xfb
    EXPECT_EQ(get_element(handle, 4, &c[4]) && c[4] == a[2], true); // Expected 3
    EXPECT_EQ(get_element(handle, 5, &c[5]) && c[5] == a[3], true); // Expected 4
    EXPECT_EQ(get_element(handle, 6, &c[6]) && c[6] == a[4], true); // Expected 5
    EXPECT_EQ(get_element(handle, 7, &c[7]) && c[7] == b[2], true); // Expected 0xfc
    
    free_array(handle);
}

/// CInterface: Test the storage of abstract data types in the dynamic array
TEST(CInterface, InsertADTValues)
{
    // Build book object that will be stored in the list
    BOOK book;
    book.book_id = 1;
    strcpy(book.title, "Algorithms");
    strcpy(book.author, "Robert Sedgewick");
    strcpy(book.subject, "Computer Programming Structured Design");

    // Initialize the dynamic array to store one book
    DYNARRAY_HANDLE handle = init_array(1, sizeof(BOOK));
    EXPECT_EQ(set_element(handle, 0, &book), true);

    BOOK book2;
    EXPECT_EQ(get_element(handle, 0, &book2), true);

    EXPECT_EQ(book.book_id, book2.book_id);
    EXPECT_EQ(strcmp(book.title, book2.title), 0);
    EXPECT_EQ(strcmp(book.author, book2.author), 0);
    EXPECT_EQ(strcmp(book.subject, book2.subject), 0);

    // De-allocate the array
    free_array(handle);
}
