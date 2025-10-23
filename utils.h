#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "sorting.h"

SortingAlgs::Algorithm algo = SortingAlgs::Algorithm::StdSort;

// randomness patterns in data
// -> nearly will always have 9 unsorted
enum class Pattern
{
    Random,
    Reverse,
    NearlySorted,
    ManyDuplicates
};

// ----------------------------------------------------------------------------
// Benchmark configuration options:
//  - n:       Number of elements in the test dataset.
//  - seed:    Random seed used for reproducible data generation.
//  - pattern: Distribution pattern of the input data (e.g., random, reverse,
//             nearly sorted, many duplicates).
//  - trials:  Number of independent runs for each algorithm; the best, average,
//             and worst times are reported.
//  - algo:    Sorting algorithm to benchmark (e.g., std::sort, insertion_sort).
// ----------------------------------------------------------------------------
struct Options
{
    std::size_t n = 65u * 1024u;
    uint64_t seed = std::random_device{}();
    Pattern pattern = Pattern::Random;
    int trials = 3;
    SortingAlgs::Algorithm algo = SortingAlgs::Algorithm::StdSort;
};

static Pattern parse_pattern(const std::string &s)
{
    if (s == "random")
        return Pattern::Random;
    if (s == "reverse")
        return Pattern::Reverse;
    if (s == "nearly")
        return Pattern::NearlySorted;
    if (s == "dups")
        return Pattern::ManyDuplicates;
    throw std::runtime_error("Unknown pattern: " + s);
}
static void print_usage()
{
    std::cout << "Sorting Algorithm Benchmark\n"
                 "----------------------------\n"
                 "Usage:\n"
                 "  sorter [options]\n\n"
                 "Options:\n"
                 "  --n <N>             Number of elements in the dataset "
                 "(default: 65536)\n"
                 "  --seed <S>          Random seed for reproducibility "
                 "(default: random_device)\n"
                 "  --pattern <P>       Data pattern: random | reverse | nearly | dups "
                 "(default: random)\n"
                 "  --trials <T>        Number of trials to average timing results "
                 "(default: 3)\n"
                 "  --algo <A>          Sorting algorithm: std | insertion | merge "
                 "(default: std)\n"
                 "  -h, --help          Show this help message and exit\n\n"
                 "Examples:\n"
                 "  sorter --n 100000 --pattern nearly --algo insertion\n"
                 "  sorter --pattern reverse --trials 5\n\n";
}

static Options parse_args(int argc, char **argv)
{
    Options opt;

    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        auto pick = [&](const std::string &flag)
        {
            if (a == flag && i + 1 < argc)
                return std::string(argv[++i]);
            return std::string();
        };

        if (auto v = pick("--n"); !v.empty())
        {
            opt.n = stoull(v);
        }
        else if (auto v = pick("--seed"); !v.empty())
        {
            opt.seed = stoull(v);
        }
        else if (auto v = pick("--pattern"); !v.empty())
        {
            opt.pattern = parse_pattern(v);
        }
        else if (auto v = pick("--trials"); !v.empty())
        {
            opt.trials = stoi(v);
        }
        else if (auto v = pick("--algo"); !v.empty())
        {
            opt.algo = SortingAlgs::parse_algorithm(v);
        }
        else if (a == "-h" || a == "--help")
        {
            print_usage();
            std::exit(0);
        }
        else
        {
            std::cerr << "Unknown option: " << a << "\n\n";
            print_usage();
            std::exit(1);
        }
    }

    return opt;
}

static std::vector<int> make_data(std::size_t n, uint64_t seed, Pattern p)
{
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<int> dist(-1'000'000, 1'000'000);
    std::vector<int> v(n);

    switch (p)
    {
    case Pattern::Random:
        for (auto &x : v)
            x = dist(rng);
        break;
    case Pattern::Reverse:
        for (std::size_t i = 0; i < n; ++i)
            v[i] = static_cast<int>(n - 1 - i);
        break;
    case Pattern::NearlySorted:
    {
        if (n < 2)
        { // nothing to do
            for (std::size_t i = 0; i < n; ++i)
                v[i] = static_cast<int>(i);
            break;
        }

        // Start sorted
        for (std::size_t i = 0; i < n; ++i)
            v[i] = static_cast<int>(i);

        // Choose a small k: 2..min(9, n)
        const std::size_t K_MAX = 9;
        const std::size_t k_hi = std::min<std::size_t>(K_MAX, n);
        std::uniform_int_distribution<std::size_t> kdist(2, k_hi);
        std::size_t k = kdist(rng);

        // Pick k distinct indices
        std::uniform_int_distribution<std::size_t> pos(0, n - 1);
        std::vector<std::size_t> idx;
        idx.reserve(k);
        while (idx.size() < k)
        {
            std::size_t i = pos(rng);
            if (std::find(idx.begin(), idx.end(), i) == idx.end())
                idx.push_back(i);
        }
        std::sort(idx.begin(), idx.end()); // stable & deterministic application

        // Extract values at those positions, rotate them, write back
        std::vector<int> vals;
        vals.reserve(k);
        for (auto i : idx)
            vals.push_back(v[i]);

        // Rotate by 1 so every chosen position is wrong (ensures unsortedness)
        std::rotate(vals.begin(), vals.begin() + 1, vals.end());

        for (std::size_t t = 0; t < k; ++t)
            v[idx[t]] = vals[t];

        break;
    }
    case Pattern::ManyDuplicates:
    {
        std::uniform_int_distribution<int> small(0, 15);
        for (auto &x : v)
            x = small(rng);
        break;
    }
    }
    return v;
}

static bool check_sorted(const std::vector<int> &v)
{
    return std::is_sorted(v.begin(), v.end());
}

static double time_one(SortingAlgs::Algorithm algo, const std::vector<int> &base)
{
    std::vector<int> a = base;
    auto t0 = std::chrono::high_resolution_clock::now();
    SortingAlgs::sort(a, algo); // <--- dispatch here
    auto t1 = std::chrono::high_resolution_clock::now();
    if (!check_sorted(a))
        throw std::runtime_error("Result not sorted!");
    std::chrono::duration<double, std::milli> ms = t1 - t0;
    return ms.count();
}

std::string algo_name(SortingAlgs::Algorithm algo)
{
    std::string name = "";

    switch (algo)
    {
    case SortingAlgs::Algorithm::Insertion:
        name = "insertion";
        break;
    case SortingAlgs::Algorithm::Merge:
        name = "merge";
        break;
    case SortingAlgs::Algorithm::StdSort:
        name = "std::sort";
        break;
    default:
        name = "unsupported";
    }

    return name;
}