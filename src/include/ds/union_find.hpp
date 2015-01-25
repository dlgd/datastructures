#ifndef DATASTRUCTURES_UNION_FIND_HPP
#define DATASTRUCTURES_UNION_FIND_HPP

#include <algorithm>
#include <cstddef>
#include <vector>

namespace ds
{

class union_find
{
public:
   typedef int site_t;

   union_find(std::size_t size):
      m_count(size),
      m_index(size),
      m_size(size, 1)
   {
      for (int i = 0; i < size; ++i)
         m_index[i] = i;
   }

   site_t find(site_t p)
   {
      while (p != m_index[p])
      {
         m_index[p] = m_index[m_index[p]];
         p = m_index[p];
      }

      return p;
   }

   void connect(site_t p, site_t q)
   {
      site_t rp = find(p);
      site_t rq = find(q);

      if (rp == rq)
         return;

      if (m_size[rp] < m_size[rq])
         std::swap(rp, rq);

      m_index[rq] = rp;
      m_size[rp] += m_size[rq];

      --m_count;
   }

   bool connected(site_t p, site_t q)
   {
      return find(p) == find(q);
   }

   std::size_t count() const
   {
      return m_count;
   }

private:
   std::size_t m_count;
   std::vector<site_t> m_index; // link to the parent site
   std::vector<size_t> m_size; // relevant for root sites
};

}

#endif
