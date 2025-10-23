#pragma once
#include <vector>
#include <cstddef>
#include <string>

using std::vector;

class SortingAlgs
{
public:
    // -> sorting algorithms that this class implements
    enum class Algorithm
    {
        StdSort,
        Insertion,
        Merge
    };

    // landing funciton
    // --> dispatches based on Algorithm
    static void sort(vector<int> &a, Algorithm algo);
    static void merge_sort_inplace_(vector<int> &a);

    // implementations
    static void insertion_sort_(vector<int> &a);

    // helpers
    SortingAlgs() = default;
    std::size_t data_len() const { return data_len_; }
    void data_len(std::size_t len) { data_len_ = len; }
    static Algorithm parse_algorithm(const std::string &s);

private:
    std::size_t data_len_ = 0;

    static vector<int> merge_sort_(const vector<int> &a);
    static vector<int> merge_(const vector<int> &left, const vector<int> &right);
};
