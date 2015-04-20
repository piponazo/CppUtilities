#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

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

TEST(CppSetDifference, shouldFillEmptyDifferenceVector)
{
    vector<int> all {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> selected {2, 4, 5, 7, 9};
    vector<int> toDelete(5);

    std::set_difference(all.begin(), all.end(), selected.begin(), selected.end(), toDelete.begin());

	ASSERT_FALSE(toDelete.empty());
	ASSERT_EQ(toDelete.size(), 5);
}
