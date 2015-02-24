#include <ds/sort.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <autocheck/autocheck.hpp>

namespace ac = autocheck;

namespace
{

struct selection_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::selection_sort(begin, end);
   }
};

struct insertion_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::insertion_sort(begin, end);
   }
};

struct shell_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::shell_sort(begin, end);
   }
};

struct merge_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::merge_sort(begin, end);
   }
};


struct quick_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::quick_sort(begin, end);
   }
};


struct heap_sort_t
{
   template<typename T>
   static void apply(T begin, T end)
   {
      ds::heap_sort(begin, end);
   }
};


using sort_funs_t = testing::Types<selection_sort_t, insertion_sort_t,
                                   shell_sort_t, merge_sort_t, quick_sort_t,
                                   heap_sort_t>;


template <typename CtnType>
void print_ctn(const CtnType& xs)
{
   std::copy(std::begin(xs), std::end(xs),
             std::ostream_iterator<decltype(*std::begin(xs))>(std::cout, ", "));
   std::cout << std::endl;
}

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

template <typename SortFun, typename CtnType, std::size_t max_tests = 100>
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
