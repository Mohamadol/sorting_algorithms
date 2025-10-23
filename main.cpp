#include "utils.h"
#include "sorting.h"

int main(int argc, char **argv)
{

    // -> for faster std::cout
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // -> parse user args
    Options opt = parse_args(argc, argv);

    // -> display test settings
    std::cout << "n=" << opt.n
              << " seed=" << opt.seed
              << " pattern=" << (opt.pattern == Pattern::Random ? "random" : opt.pattern == Pattern::Reverse    ? "reverse"
                                                                         : opt.pattern == Pattern::NearlySorted ? "nearly"
                                                                                                                : "dups")
              << " trials=" << opt.trials
              << " algo=" << (opt.algo == SortingAlgs::Algorithm::StdSort ? "std" : "insertion")
              << "\n";

    // -> init test data
    std::vector<int> base = make_data(opt.n, opt.seed, opt.pattern);

    // -> sort & time
    double best = 1e300, worst = -1.0, sum = 0.0;
    for (int t = 0; t < opt.trials; ++t)
    {
        double ms = time_one(opt.algo, base);
        best = std::min(best, ms);
        worst = std::max(worst, ms);
        sum += ms;
    }
    double avg = sum / opt.trials;

    // -> print test results
    std::cout << std::left << std::setw(20) << "Algorithm"
              << std::right << std::setw(12) << "best(ms)"
              << std::setw(12) << "avg(ms)"
              << std::setw(12) << "worst(ms)" << "\n";

    std::cout << std::left << std::setw(20)
              << (opt.algo == SortingAlgs::Algorithm::StdSort ? "std::sort" : "insertion_sort")
              << std::right << std::setw(12) << std::fixed << std::setprecision(3) << best
              << std::setw(12) << avg
              << std::setw(12) << worst << "\n";

    return 0;
}
