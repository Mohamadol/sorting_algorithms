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
    else if (s == "merge")
    {
        return Algorithm::Merge;
    }
    else
    {
        throw std::runtime_error("Unknown --algo: " + s);
    }
}

void SortingAlgs::sort(vector<int> &a, Algorithm algo)
{
    switch (algo)
    {
    case Algorithm::StdSort:
        std::sort(a.begin(), a.end());
        break;
    case Algorithm::Insertion:
        insertion_sort_(a);
        break;
    case Algorithm::Merge:
        merge_sort_inplace_(a);
        break;
    default:
        throw std::runtime_error("Unimplemented algorithm");
    }
}

void SortingAlgs::insertion_sort_(vector<int> &a)
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

void SortingAlgs::merge_sort_inplace_(vector<int> &a)
{
    a = merge_sort_(a);
}

vector<int> SortingAlgs::merge_sort_(const vector<int> &a)
{
    size_t n = a.size();

    if (n == 1)
    {
        return a;
    }

    vector<int> a_left(a.begin(), a.begin() + n / 2);
    vector<int> a_right(a.begin() + n / 2, a.end());

    return merge_(merge_sort_(a_left), merge_sort_(a_right));
}

vector<int> SortingAlgs::merge_(const vector<int> &left, const vector<int> &right)
{
    size_t l_size = left.size();
    size_t r_size = right.size();

    vector<int> merged(l_size + r_size);
    size_t l_used = 0, r_used = 0;
    for (size_t i = 0; i < l_size + r_size; i++)
    {
        if (l_used == l_size)
        {
            merged[i] = right[r_used++];
        }
        else if (r_used == r_size)
        {
            merged[i] = left[l_used++];
        }
        else if (left[l_used] <= right[r_used])
        {
            merged[i] = left[l_used++];
        }
        else
        {
            merged[i] = right[r_used++];
        }
    }
    return merged;
}
