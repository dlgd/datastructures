#include <ds/union_find.hpp>

#include <fstream>
#include <gtest/gtest.h>


static const char* tinyTestFile = nullptr;
static const char* mediumTestFile = nullptr;
static const char* largeTestFile = nullptr;


static std::size_t components_count(std::ifstream& ifs)
{
   size_t size;
   ifs >> size;
   ds::union_find uf(size);

   int p, q;
   while (ifs >> p >> q)
   {
      uf.connect(p, q);
   }

   return uf.count();
}

TEST(union_find, init)   
{
   const size_t N = 5;
   ds::union_find uf(N);

   EXPECT_EQ(N, uf.count());

   for (int i = 0; i < static_cast<int>(N); ++i) 
   {
      EXPECT_EQ(i, uf.find(i));
   }
}

TEST(union_find, one_connection)
{
   ds::union_find uf(2);

   uf.connect(0, 1);
   EXPECT_EQ(1u, uf.count());
   EXPECT_EQ(uf.find(0), uf.find(1));
   EXPECT_TRUE(uf.connected(0, 1));
}

TEST(union_find, connect_connnected)
{
   ds::union_find uf(2);

   uf.connect(0, 1);
   EXPECT_EQ(1u, uf.count());
   uf.connect(0, 1);
   EXPECT_EQ(1u, uf.count());
}

TEST(union_find, reflexivity)
{
   ds::union_find uf(2);

   EXPECT_TRUE(uf.connected(0, 0));
   EXPECT_TRUE(uf.connected(1, 1));

   uf.connect(0, 1);

   EXPECT_TRUE(uf.connected(0, 0));
   EXPECT_TRUE(uf.connected(1, 1));
}

TEST(union_find, symmetry)
{
   ds::union_find uf(2);

   uf.connect(0, 1);
   EXPECT_TRUE(uf.connected(0, 1));
   EXPECT_TRUE(uf.connected(1, 0));
}

TEST(union_find, transitivity1)
{
   ds::union_find uf(3);

   uf.connect(0, 1);
   uf.connect(0, 2);
   EXPECT_TRUE(uf.connected(1, 2));
}

TEST(union_find, transitivity2)
{
   ds::union_find uf(4);

   uf.connect(0, 1);
   uf.connect(1, 2);
   EXPECT_TRUE(uf.connected(0, 2));
}

TEST(union_find, tiny)
{
   std::ifstream ifs(tinyTestFile);
   if (ifs.is_open()) 
   {
      EXPECT_EQ(2u, components_count(ifs));
   }
}

TEST(union_find, medium)
{
   std::ifstream ifs(mediumTestFile);
   if (ifs.is_open()) 
   {
      EXPECT_EQ(3u, components_count(ifs));
   }
}

TEST(union_find, large)
{
   std::ifstream ifs(largeTestFile);
   if (ifs.is_open()) 
   {
      EXPECT_EQ(6u, components_count(ifs));
   }
}

int main(int argc, char** argv) 
{
   ::testing::InitGoogleTest(&argc, argv);
 
   if (argc == 4)
   {
      tinyTestFile = argv[1];
      mediumTestFile = argv[2];
      largeTestFile = argv[3];
   }

   return RUN_ALL_TESTS();
}
