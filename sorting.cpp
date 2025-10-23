#include "sorting.h"
#include <algorithm>
#include <stdexcept>

using Algorithm = SortingAlgs::Algorithm;

SortingAlgs::Algorithm SortingAlgs::parse_algorithm(const std::string &s)
{
    if (s == "std")
    {
        return Algorithm::StdSort;
    }
    else if (s == "insertion")
    {
        return Algorithm::Insertion;
    }
    else
    {
        throw std::runtime_error("Unknown --algo: " + s);
    }
}

void SortingAlgs::sort(std::vector<int> &a, Algorithm algo)
{
    switch (algo)
    {
    case Algorithm::StdSort:
        std::sort(a.begin(), a.end());
        break;
    case Algorithm::Insertion:
        insertion_sort_(a);
        break;
    default:
        throw std::runtime_error("Unimplemented algorithm");
    }
}

void SortingAlgs::insertion_sort_(std::vector<int> &a)
{
    const std::size_t n = a.size();
    for (std::size_t i = 1; i < n; ++i)
    {
        int key = a[i];
        std::size_t j = i;
        while (j > 0 && a[j - 1] > key)
        {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}
