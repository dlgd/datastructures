#include <ds/sort.hpp>

#include <algorithm>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <autocheck/autocheck.hpp>

namespace ac = autocheck;

namespace
{

struct insertion_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::insertion_sort(begin, end);
   }
};

struct selection_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::insertion_sort(begin, end);
   }
};

using sort_funs_t = testing::Types<insertion_sort_t, selection_sort_t>;


template <typename SortFun, typename CtnType>
static bool check_sort(CtnType& xs)
{
   SortFun::apply(std::begin(xs), std::end(xs));
   return std::is_sorted(std::begin(xs), std::end(xs));
}

template <typename SortFun>
struct prop_sort_t
{
   template <typename T>
   bool operator() (std::vector<T>& xs) const
   {
      return check_sort<SortFun>(xs);
   }
};

template <typename SortFun, typename CtnType, std::size_t max_tests = 1000>
static void check_prop_sort()
{
   ac::check<CtnType>(prop_sort_t<SortFun>(), max_tests,
                      ac::make_arbitrary<CtnType>(), ac::gtest_reporter());
}


// Basic unitary tests

template <class T>
class sort_basic_test_t : public testing::Test
{
};

TYPED_TEST_CASE(sort_basic_test_t, sort_funs_t);

TYPED_TEST(sort_basic_test_t, empty_vector)
{
   std::vector<int> xs;
   EXPECT_TRUE(check_sort<TypeParam>(xs));
}

TYPED_TEST(sort_basic_test_t, small_unsorted_vector)
{
   std::vector<int> xs{3, 2, 1};
   EXPECT_TRUE(check_sort<TypeParam>(xs));
}

TYPED_TEST(sort_basic_test_t, empty_list)
{
   std::list<int> xs;
   EXPECT_TRUE(check_sort<TypeParam>(xs));
}

TYPED_TEST(sort_basic_test_t, small_unsorted_list)
{
   std::list<int> xs{3, 2, 1};
   EXPECT_TRUE(check_sort<TypeParam>(xs));
}


// Auto check tests

template <class T>
class sort_auto_check_t : public testing::Test
{
};

TYPED_TEST_CASE(sort_auto_check_t, sort_funs_t);

TYPED_TEST(sort_auto_check_t, vector_int)
{
   check_prop_sort<TypeParam, std::vector<int>>();
}

TYPED_TEST(sort_auto_check_t, vector_string)
{
   check_prop_sort<TypeParam, std::vector<std::string>>();
}

}
