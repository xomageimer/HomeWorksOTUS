#include "pch.h"
#include "allocator.h"
#include "List.h"
#include "profile.h"
#include <gtest/gtest.h>

using namespace std;

TEST (Allocator, allocate){
    vector<int> v;
    for (size_t i = 0; i < 100; i++)
        v.emplace_back(i);
    auto m_reserve = std::map<int, vector<int>, std::less<int>, PoolAllocator<std::pair<const int, vector<int>>, 10'000>>{};
    auto m_std = std::map<int, vector<int>> {};

    {
        Code_Timer(custom alloc);

        for (size_t i = 0; i < 100'000; i++) {
            m_reserve.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(v));
        }
    }
    {
        Code_Timer(std alloc);

        for (size_t i = 0; i < 100'000; i++)
        {
            m_std.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(v));
        }
    }

    ASSERT_EQ(m_reserve.size(), m_std.size());
    ASSERT_FALSE(m_reserve.empty());
}


TEST (LIST, RANGE_COUNSTRUCTOR){
    vector<int> range {1,2,3,4,5,6,7,8,13};
    size_t sum = 0;
    size_t sum2 = 0;
    for (auto & i : range)
        sum += i;

    List<int> list (range.begin(), range.end());
    for (auto & i : list)
        sum2 += i;

    ASSERT_EQ(sum, sum2);
}


TEST (LIST, POP_BACK){
    List<int> l;

    l.emplace_front(5);
    l.emplace_front(6);
    l.pop_front();

    ASSERT_EQ(l.size(), 1);

    l.pop_front();

    ASSERT_EQ(l.size(), 0);
}
