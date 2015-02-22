#ifndef DATASTRUCTURES_PRIORITY_QUEUE_HPP
#define DATASTRUCTURES_PRIORITY_QUEUE_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>

namespace ds
{

template<typename ElementType, typename LessType = std::less<ElementType>>
class priority_queue
{
private:
   using Elements = std::vector<ElementType>;

public:
   using size_type = typename Elements::size_type;
   using value_type = ElementType;

   priority_queue(const LessType& less):
      m_elements(1),
      m_less(less)
   {}

   priority_queue():
      m_elements(1)
   {}

   void insert(const ElementType& element)
   {
      m_elements.push_back(element);
      swim(m_elements.size() - 1);
   }

   void del_max()
   {
      if (empty())
         return;

      auto last_elt = std::prev(m_elements.end());
      std::swap(*last_elt, m_elements[1]);
      m_elements.erase(last_elt, m_elements.end());
      sink(1);
   }

   const ElementType& max() const
   {
      return m_elements.at(1);
   }

   size_type size() const { return m_elements.size() - 1; };

   bool empty() const { return size() == 0; }

private:
   Elements m_elements;
   LessType m_less;

   void swim(size_type i)
   {
      if (i <= 1)
         return;

      const auto p = i / 2;
      if (m_less(m_elements[p], m_elements[i]))
      {
         std::swap(m_elements[p], m_elements[i]);
         swim(p);
      }
   }

   void sink(size_type i)
   {
      const auto s = m_elements.size();
      if (s <= 2 * i)
         return;

      auto m = 2 * i;
      if (2 * i + 1 < s && m_less(m_elements[2 * i], m_elements[2 * i + 1]))
         m = 2 * i + 1;

      if (m_less(m_elements[i], m_elements[m]))
      {
         std::swap(m_elements[i], m_elements[m]);
         sink(m);
      }
   }

};

}

#endif
