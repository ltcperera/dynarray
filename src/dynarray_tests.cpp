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
#include "gtest/gtest.h"
#include "dynarray.hpp"

// Prevent C++ compiler from mangling C names
//   so that linker is able to find the functions
extern "C"
{
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

/// Tests swap function by passing two variables
///  and testing if their values were swapped.
TEST(Utility, swap)
{
  int a = 5;
  int b = 10;
  swap(a, b);
  EXPECT_EQ(a, 10);
  EXPECT_EQ(b, 5);
}

/// Tests default constructor that constructs a 0 length array
TEST(CPPInterface, ConstructionZeroLength)
{
  dynarray<int> arr;
  EXPECT_EQ(arr.array_capacity(), 0);
}

/// Tests constructor with specified array length
TEST(CPPInterface, ConstructionSpesifiedLength)
{
  dynarray<int> arr(20);
  EXPECT_EQ(arr.array_capacity(), 20);
}

/// Tests insertion of elements with resize operation
TEST(CPPInterface, ResizeOperationInsertAtBeginning)
{
  dynarray<int> arr;

  // Insert element at 0th index
  arr.insert_element(0, 1);

  // Expect the array to resize by 1
  EXPECT_EQ(arr.array_capacity(), 1);
}

/// Tests insertion of values of abstract data types
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

// ======================
// Tests for C-Interfaces
// ======================

/// Test construction of a dynamic array using the C interface
///   The test allocates the array and checks its capacity.
TEST(CInterface, Construction)
{
  // Allocate dynamic array for 10 integers
  DYNARRAY_HANDLE handle = init_array(10, sizeof(int));
  EXPECT_EQ(array_capacity(handle), 10);

  // De-allocate the array
  free_array(handle);
}

/// Test the storage of abstract data types in the dynamic array
TEST(CInterface, AbstractDataTypes)
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

  free_array(handle);
  //EXPECT_EQ(sizeof(BOOK), 204);
}
