#include <ds/bs_tree.hpp>
#include <ds/rb_tree.hpp>

#include <gtest/gtest.h>

template<typename TreeType, typename KeyType, typename ValueType>
static void check_get(TreeType& t, const KeyType& key, const ValueType& value)
{
   auto v = t.get(key);
   ASSERT_TRUE(v != nullptr);
   EXPECT_EQ(value, *v);
}

template<typename TreeType, typename KeyType>
static void check_remove(TreeType& t, const KeyType& key)
{
   auto s = t.size();
   t.remove(key);
   EXPECT_EQ(s - 1, t.size());
   EXPECT_EQ(nullptr, t.get(key));
}

TEST(bs_tree, basic)
{
   ds::bs_tree_t<int, int> t;
   EXPECT_EQ(0, t.size());

   t.put(1, 42);
   EXPECT_EQ(1, t.size());
   check_get(t, 1, 42);

   t.put(1, 51);
   EXPECT_EQ(1, t.size());
   check_get(t, 1, 51);

   EXPECT_EQ(nullptr, t.get(6));

   t.put(-8, 18);
   t.put(-12, 13);
   t.put(-3, 13);
   t.put(6, 23);
   EXPECT_EQ(5, t.size());
   check_get(t, -8, 18);
   check_get(t, -12, 13);
   check_get(t, -3, 13);
   check_get(t, 6, 23);

   check_remove(t, -12);
   check_remove(t, 1);
   check_remove(t, -3);
   check_remove(t, 6);
   check_remove(t, -8);
}

TEST(rb_tree, basic)
{
   ds::rb_tree_t<int, int> t;
   EXPECT_EQ(0, t.size());

   t.put(1, 42);
   EXPECT_EQ(1, t.size());
   check_get(t, 1, 42);

   t.put(1, 51);
   EXPECT_EQ(1, t.size());
   check_get(t, 1, 51);

   EXPECT_EQ(nullptr, t.get(6));

   t.put(-8, 18);
   t.put(-12, 13);
   t.put(-3, 13);
   t.put(6, 23);
   EXPECT_EQ(5, t.size());
   check_get(t, -8, 18);
   check_get(t, -12, 13);
   check_get(t, -3, 13);
   check_get(t, 6, 23);
}


