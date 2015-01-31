#ifndef DATASTRUCTURES_UNION_FIND_HPP
#define DATASTRUCTURES_UNION_FIND_HPP

#include <cstddef>
#include <vector>

namespace ds
{

class union_find
{
public:
   using site_t = int;

   union_find(std::size_t size);

   site_t find(site_t p);

   void connect(site_t p, site_t q);

   bool connected(site_t p, site_t q);

   std::size_t count() const;

private:
   std::size_t m_count;
   std::vector<site_t> m_index; // link to the parent site
   std::vector<size_t> m_size; // relevant for root sites
};

}

#endif
