#include "ds/priority_queue.hpp"

#include <functional>
#include <vector>
#include <set>

#include <gtest/gtest.h>

#include <autocheck/autocheck.hpp>

namespace ac = autocheck;

namespace
{

TEST(priority_queue, insert)
{
   ds::priority_queue<int> pq;

   EXPECT_TRUE(pq.empty());

   EXPECT_EQ(0, pq.size());

   pq.insert(42);
   EXPECT_EQ(42, pq.max());
   EXPECT_FALSE(pq.empty());
   EXPECT_EQ(1, pq.size());

   pq.insert(3);
   EXPECT_EQ(42, pq.max());
   EXPECT_EQ(2, pq.size());

   pq.insert(51);
   EXPECT_EQ(51, pq.max());
   EXPECT_EQ(3, pq.size());
}

TEST(priority_queue, del)
{
   ds::priority_queue<int> pq;

   pq.insert(42);
   EXPECT_EQ(42, pq.max());

   pq.del_max();
   EXPECT_TRUE(pq.empty());
   EXPECT_EQ(0, pq.size());

   pq.insert(42);
   pq.insert(8);
   pq.insert(56);
   pq.insert(2);
   EXPECT_EQ(56, pq.max());

   pq.del_max();
   EXPECT_EQ(42, pq.max());
}

struct prop_max_t
{
   template <typename T>
   bool operator() (std::vector<T>& xs) const
   {
      const std::multiset<T> s(xs.begin(), xs.end());
      ds::priority_queue<T> pq;
      for (const auto& i : s)
         pq.insert(i);

      if (pq.size() != s.size())
         return false;

      for (auto it = s.rbegin(); it != s.rend(); ++it)
      {
         if (*it != pq.max())
            return false;
         pq.del_max();
      }

      return true;
   }
};


TEST(priority_queue, prop)
{
   using ctn_type = std::vector<int>;
   ac::check<ctn_type>(prop_max_t(), 1000, ac::make_arbitrary<ctn_type>(),
                       ac::gtest_reporter());
}

}
