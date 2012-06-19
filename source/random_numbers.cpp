#include <stdlib.h>
#include <vector>
#include <algorithm>

// Improvement: return a pointer to the vector instead of the vector itself

std::vector<int> get_random_int_vector(int length,
                                  int lower_limit,
                                  int upper_limit)
{
    std::vector<int> v;

    for(int i = lower_limit; i < upper_limit; i++)
    {
        v.push_back(i);
    }

    random_shuffle(v.begin(), v.end());

    //std::vector<int>::const_iterator first = v.begin() + lower_limit;
    std::vector<int>::const_iterator first = v.begin();
    //std::vector<int>::const_iterator last = v.begin() + upper_limit;
    std::vector<int>::const_iterator last = v.begin() + length;
    std::vector<int> newVec(first, last);

    return newVec;
}

