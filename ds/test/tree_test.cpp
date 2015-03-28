#include <ds/bs_tree.hpp>
#include <ds/rb_tree.hpp>

#include <gtest/gtest.h>

#include <autocheck/autocheck.hpp>

#include <map>
#include <set>
#include <utility>

namespace ac = autocheck;


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

struct rb_tree_factory_t
{
   template <typename T>
   static ds::rb_tree_t<T, T> instance()
   {
      return ds::rb_tree_t<T, T>();
   }
};

struct bs_tree_factory_t
{
   template <typename T>
   static ds::bs_tree_t<T, T> instance()
   {
      return ds::bs_tree_t<T, T>();
   }
};

template <typename TreeFactoryType>
struct prop_insert_t
{
   template <typename T>
   bool operator() (const std::vector<T>& xs) const
   {
      auto t = TreeFactoryType::template instance<T>();
      for (auto x : xs)
      {
         t.put(x, x);
      }

      const std::set<T> ss(xs.begin(), xs.end());
      return t.size() == ss.size();
   }
};

template <typename TreeFactoryType>
struct prop_insert_delete_t
{
   template <typename T>
   bool operator() (const std::vector<T>& xs) const
   {
      auto t = TreeFactoryType::template instance<T>();

      for (const auto& x : xs)
      {
         t.put(x, x);
      }

      const std::set<T> ss(xs.begin(), xs.end());
      if (ss.size() != xs.size())
         return true;

      for (const auto& x : ss)
      {
         const auto v = t.get(x);
         if (!v || *v != x)
            return false;

         const auto s = t.size();
         t.remove(x);
         if (t.size() != s - 1)
            return false;
      }

      return t.size() == 0;
   }
};

template <typename PropertyType, typename ValueType,
          std::size_t max_tests = 100>
static void check_prop()
{
   using ctn_t = std::vector<ValueType>;
   ac::check<ctn_t>(PropertyType(), max_tests, ac::make_arbitrary<ctn_t>(),
                    ac::gtest_reporter());
}

using tree_factory_types_t =
   testing::Types<bs_tree_factory_t, rb_tree_factory_t>;

template <class T>
class tree_test_t : public testing::Test
{
};

TYPED_TEST_CASE(tree_test_t, tree_factory_types_t);

TYPED_TEST(tree_test_t, basic)
{
   auto t = TypeParam::template instance<int>();
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

TYPED_TEST(tree_test_t, insert_int)
{
   check_prop<prop_insert_t<TypeParam>, int>();
}

TYPED_TEST(tree_test_t, insert_string)
{
   check_prop<prop_insert_t<TypeParam>, std::string>();
}

TYPED_TEST(tree_test_t, insert_delete_int)
{
   check_prop<prop_insert_delete_t<TypeParam>, int>();
}

TYPED_TEST(tree_test_t, insert_delete_string)
{
   check_prop<prop_insert_delete_t<TypeParam>, std::string>();
}
