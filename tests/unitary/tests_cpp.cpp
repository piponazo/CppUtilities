#include <gtest/gtest.h>
#include <vector>

using namespace std;

TEST(CppVectorCapacity, capacityShouldDifferFromSize)
{
    vector<int> v;
    v.reserve(100);
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
	ASSERT_NE(v.capacity(), 0);
}

TEST(CppVectorCapacity, capacityShouldNotBe0AfterClear)
{
    vector<int> v {1,2,3};
    v.clear();
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
	ASSERT_NE(v.capacity(), 0);
}

TEST(CppVectorCapacity, capacityShouldtBe0AfterTrickSwap)
{
    vector<int> v {1,2,3};
    vector<int>().swap(v);
	ASSERT_TRUE(v.empty());
	ASSERT_EQ(v.size(), 0);
	ASSERT_EQ(v.capacity(), 0);
}

//TEST(CppVectorCapacity, capacityShouldtNotBe0AfterTrickDestructor)
//{
//    vector<int> v {1,2,3};
//    v.~vector();
//	EXPECT_FALSE(v.empty());
//	EXPECT_NE(v.size(), 0);
//	EXPECT_NE(v.capacity(), 0);
//}
