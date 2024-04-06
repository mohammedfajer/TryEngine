
#include "defines.h"

// #############################################################################
//                           Array
// #############################################################################

/*
  Useage

  Array<int, 10> integer_array;
  
  @Add
  integer_array.add(2);
  integer_array.add(5);
  integer_array.add(6);


  [2 5 6]

  @Remove and swap with last
  integer_array.remove_idx_and_swap(1);
  
  [2 6]

  if(!integer_array.is_full())
  {
    integer_array.clear();
  }

*/

template<typename T, int N>
struct Array
{
  static constexpr int maxElements = N;
  int count = 0;
  T elements[N];

  T& operator[](int idx)
  {
    SM_ASSERT(idx >= 0, "idx negative!");
    SM_ASSERT(idx < count, "Idx out of bounds!");
    return elements[idx];
  }

  int add(T element)
  {
    SM_ASSERT(count < maxElements, "Array Full!");
    elements[count] = element;
    return count++;
  }

  void remove_idx_and_swap(int idx)
  {
    SM_ASSERT(idx >= 0, "idx negative!");
    SM_ASSERT(idx < count, "idx out of bounds!");
    elements[idx] = elements[--count];
  }

  void clear()
  {
    count = 0;
  }

  bool is_full()
  {
    return count == N;
  }
};