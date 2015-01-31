#ifndef DATASTRUCTURES_SORT_HPP
#define DATASTRUCTURES_SORT_HPP

#include <algorithm>
#include <functional>

namespace ds
{

template <typename IteratorType, typename LessType>
void selection_sort(IteratorType begin, IteratorType end, LessType less)
{
   for (IteratorType i = begin; i != end; ++i)
   {
      IteratorType min = i;
      for (IteratorType j = std::next(i); j != end; ++j)
      {
         if (less(*j, *min))
            min = j;
      }
      std::iter_swap(i, min);
   }
}

template <typename IteratorType>
void selection_sort(IteratorType begin, IteratorType end)
{
   selection_sort(begin, end, std::less<decltype(*begin)>());
}

template <typename IteratorType, typename LessType>
void insertion_sort(IteratorType begin, IteratorType end, LessType less)
{
   for (IteratorType i = begin; i != end; ++i)
   {
      for (IteratorType j = i; j != begin && less(*j, *std::prev(j)); --j)
      {
         std::iter_swap(j, std::prev(j));
      }
   }
}

template <typename IteratorType>
void insertion_sort(IteratorType begin, IteratorType end)
{
   insertion_sort(begin, end, std::less<decltype(*begin)>());
}



}

#endif
