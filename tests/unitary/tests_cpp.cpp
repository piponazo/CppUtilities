#include <catch2/catch.hpp>

#include <vector>
#include <algorithm>

using namespace std;

TEST_CASE("Vector capacityShouldDifferFromSize")
{
    vector<int> v;
    v.reserve(100);
    REQUIRE(v.empty());
    REQUIRE(v.capacity() != 0);
}

TEST_CASE("Vector capacityShouldNotBe0AfterClear")
{
    vector<int> v {1,2,3};
    v.clear();
    REQUIRE(v.empty());
    REQUIRE(v.capacity() != 0);
}

TEST_CASE("Vector capacityShouldtBe0AfterTrickSwap")
{
    vector<int> v {1,2,3};
    vector<int>().swap(v);
    REQUIRE(v.empty());
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
}

//TEST_CASE(CppVectorCapacity, capacityShouldtNotBe0AfterTrickDestructor)
//{
//    vector<int> v {1,2,3};
//    v.~vector();
//	EXPECT_FALSE(v.empty());
//	EXPECT_NE(v.size(), 0);
//	EXPECT_NE(v.capacity(), 0);
//}

TEST_CASE("Vector shouldFillEmptyDifferenceVector")
{
    vector<int> all {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> selected {2, 4, 5, 7, 9};
    vector<int> toDelete(5);

    std::set_difference(all.begin(), all.end(), selected.begin(), selected.end(), toDelete.begin());

    REQUIRE(toDelete.size() == 5);
}
