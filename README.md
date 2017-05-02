## Dynamic Array Implementation

This is an implementation of a dynamic array that allows elements to be added and removed as needed. It resizes to grow automatically as new elements are added. The array can be allocated with an initial capacity. When the capacity is reached, the size of the backing array is increased to accommodate new elements. Elements can be added or removed at the beginning, end or by index as required.

This library has both C and C++ interfaces for use.

### Operations

- Create a dynamic array of specified size
- Insert element at specified index
- Insert element at the beginning/end
- Delete element at specified index
- Delete element at beginning/end
- Get the number of elements in the array
- Get the current capacity of the array
- Returns whether a specified item exists in the array
- Returns the index of a specified item
- Returns whether or not the array is empty
- Trim array down to size
- Reserve space within the array

### Building

The project requires CMake. Once CMake is run, the script will download and build the Google Test framework as a dependency first, followed by building the tests for the dynamic array library. The final test binary is stored in the **build** directory.

~~~
  cmake .
  make
~~~
