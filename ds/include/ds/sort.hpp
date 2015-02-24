#ifndef DATASTRUCTURES_SORT_HPP
#define DATASTRUCTURES_SORT_HPP

#include <algorithm>
#include <cassert>
#include <iterator>
#include <functional>

namespace ds
{

template <typename IteratorType, typename LessType>
void selection_sort(IteratorType begin, IteratorType end, LessType less)
{
   for (auto i = begin; i != end; ++i)
   {
      auto min = i;
      for (auto j = std::next(i); j != end; ++j)
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
   for (auto i = begin; i != end; ++i)
   {
      for (auto j = i; j != begin && less(*j, *std::prev(j)); --j)
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

namespace detail
{

template <typename IteratorType>
typename std::iterator_traits<IteratorType>::difference_type
subseq_length(IteratorType begin, IteratorType end)
{
   auto n = std::distance(begin, end);
   if (n == 0)
      return 0;

   decltype(n) h = 1;
   while (h < n / 3)
   {
      h = 3 * h + 1;
   }
   return h;
}

template <typename IteratorType, typename LessType, typename Tag>
void shell_sort(IteratorType begin, IteratorType end, LessType less, Tag)
{
   for (auto h = subseq_length(begin, end); h >= 1; h /= 3)
   {
      const auto begin_h_dist = std::distance(begin, std::next(begin, h));
      for (auto i = std::next(begin, h); i != end; ++i)
      {
         auto j = i;
         auto pj = std::prev(j, h);
         while (less(*j, *pj))
         {
            std::iter_swap(j, pj);
            j = pj;

            if (std::distance(begin, j) < begin_h_dist)
               break;

            pj = std::prev(j, h);
         }
      }
   }
}

template <typename IteratorType, typename LessType>
void shell_sort(IteratorType begin, IteratorType end, LessType less,
                std::random_access_iterator_tag)
{
   for (auto h = subseq_length(begin, end); h >= 1; h /= 3)
   {
      for (auto i = std::next(begin, h); i != end; ++i)
      {
         for (auto j = i;
              j >= std::next(begin, h) && less(*j, *std::prev(j, h));
              j = std::prev(j, h))
         {
            std::iter_swap(j, std::prev(j, h));
         }
      }
   }
}

}

template <typename IteratorType, typename LessType>
void shell_sort(IteratorType begin, IteratorType end, LessType less)
{
   using category_t =
      typename std::iterator_traits<IteratorType>::iterator_category;
   detail::shell_sort(begin, end, less, category_t());
}

template <typename IteratorType>
void shell_sort(IteratorType begin, IteratorType end)
{
   shell_sort(begin, end, std::less<decltype(*begin)>());
}

namespace detail
{

template <typename IteratorType, typename LessType>
void merge(IteratorType begin, IteratorType mid, IteratorType end,
           LessType less)
{
   std::vector<typename IteratorType::value_type> aux(begin, end);
   auto m = std::next(aux.begin(), std::distance(begin, mid));
   auto i = aux.begin(), j = m;

   for (auto o = begin; o != end; ++o)
   {
      decltype(aux.begin()) p;
      if (i == m)
         p = j++;
      else if (j == aux.end())
         p = i++;
      else
         p = less(*j, *i) ? j++ : i++;

      *o = *p;
   }
}

}

template <typename IteratorType, typename LessType>
void merge_sort(IteratorType begin, IteratorType end, LessType less)
{
   const auto l = std::distance(begin, end);
   if (l <= 1)
      return;

   const auto mid = std::next(begin, l / 2);
   merge_sort(begin, mid, less);
   merge_sort(mid, end, less);
   detail::merge(begin, mid, end, less);
}

template <typename IteratorType>
void merge_sort(IteratorType begin, IteratorType end)
{
   merge_sort(begin, end, std::less<decltype(*begin)>());
}

namespace detail
{

template <typename IteratorType, typename LessType>
IteratorType partition(IteratorType begin, IteratorType end, LessType less)
{
   assert(begin != end);
   if (std::next(begin) == end)
      return begin;

   auto& v = *begin;
   auto i = begin, j = end, last = std::prev(end);
   bool crossed = false;
   while (true)
   {
      do {
         crossed |= ++i == j;
      } while (less(*i, v) && i != last);

      do {
         crossed |= i == --j;
      } while (less(v, *j) && j != begin);

      if (crossed)
         break;

      std::iter_swap(i, j);
   }

   std::swap(v, *j);
   return j;
}

}

template <typename IteratorType, typename LessType>
void quick_sort(IteratorType begin, IteratorType end, LessType less)
{
   if (begin == end)
      return;

   const auto p = detail::partition(begin, end, less);
   assert(p != end);
   quick_sort(begin, p, less);
   quick_sort(std::next(p), end, less);
}

template <typename IteratorType>
void quick_sort(IteratorType begin, IteratorType end)
{
   quick_sort(begin, end, std::less<decltype(*begin)>());
}


namespace detail
{

size_t child_index(size_t i)
{
  return 2 * (i + 1) - 1;
}

template <typename IteratorType, typename LessType>
void sink(IteratorType begin, IteratorType end, IteratorType p, LessType less)
{
   const auto l = std::distance(begin, end);
   const auto ip = std::distance(begin, p);
   const auto ic = child_index(ip);
   if (ic >= l)
      return;

   auto c = std::next(begin, ic);
   auto imax_c = ic;
   if (ic + 1 < l && less(*c, *std::next(c, 1)))
      imax_c = ic + 1;

   auto max_c = std::next(begin, imax_c);
   if (less(*p, *max_c))
   {
      std::iter_swap(p, max_c);
      sink(begin, end, max_c, less);
   }
}

}

template <typename IteratorType, typename LessType>
void heap_sort(IteratorType begin, IteratorType end, LessType less)
{
   const auto l = std::distance(begin, end);
   if (l <= 1)
      return;

   auto it = std::next(begin, l / 2 - 1);
   do
   {
      detail::sink(begin, end, it, less);
   }
   while (it-- != begin);

   it = std::prev(end);
   do
   {
      std::iter_swap(begin, it);
      detail::sink(begin, it, begin, less);
   }
   while (it-- != begin);
}

template <typename IteratorType>
void heap_sort(IteratorType begin, IteratorType end)
{
   heap_sort(begin, end, std::less<decltype(*begin)>());
}

}

#endif
