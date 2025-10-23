#pragma once
#include <vector>
#include <cstddef>
#include <string>

class SortingAlgs
{
public:
    // -> sorting algorithms that this class implements
    enum class Algorithm
    {
        StdSort,
        Insertion
    };

    // landing funciton
    // --> dispatches based on Algorithm
    static void sort(std::vector<int> &a, Algorithm algo);

    // implementations
    static void insertion_sort_(std::vector<int> &a);

    // helpers
    SortingAlgs() = default;
    std::size_t data_len() const { return data_len_; }
    void data_len(std::size_t len) { data_len_ = len; }
    static Algorithm parse_algorithm(const std::string &s);

private:
    std::size_t data_len_ = 0;
};
