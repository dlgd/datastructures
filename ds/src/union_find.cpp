#include "ds/union_find.hpp"

#include <algorithm>

namespace ds
{

union_find::union_find(std::size_t size):
   m_count(size),
   m_index(size),
   m_size(size, 1)
{
   site_t i = 0;
   std::generate(m_index.begin(), m_index.end(), [&i] () { return i++; });
}

 union_find::site_t union_find::find(site_t p)
{
   while (p != m_index[p])
   {
      m_index[p] = m_index[m_index[p]];
      p = m_index[p];
   }

   return p;
}

void union_find::connect(site_t p, site_t q)
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

bool union_find::connected(site_t p, site_t q)
{
   return find(p) == find(q);
}

std::size_t union_find::count() const
{
   return m_count;
}

}
