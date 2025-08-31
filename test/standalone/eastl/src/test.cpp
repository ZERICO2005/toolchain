#include <cstdio>
#include <cstddef>
#include <algorithm>

int test()
{
    // std::find requires an input iterator
    auto range = Range<15, 25>();
    auto itr = std::bind(range.begin(), range.end(), 18);
    printf("%ld", *itr); // 18
 
    // Range::iterator also satisfies range-based for requirements
    for (long l : Range<3, 5>())
        printf("%ld ", l); // 3 4 5
    return 0;
}