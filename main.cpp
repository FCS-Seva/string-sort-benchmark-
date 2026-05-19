#include "src/StringGenerator.h"
#include "src/StringSortTester.h"
#include "src/sorts.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>
#include <iomanip>

struct Algorithm {
    std::string name;
    std::function<void(std::vector<std::string>&)> fn;
};

static const int STEP = 100;
static const int MAX_N = 3000;
static const int RUNS = 11;

enum class ArrayType { Random, ReverseSorted, NearlySorted };

const char* arrayTypeName(ArrayType t) {
    switch (t) {
        case ArrayType::Random: return "random";
        case ArrayType::ReverseSorted: return "reverse";
        case ArrayType::NearlySorted: return "nearly";
    }
    return "?";
}

struct Datasets {
    std::vector<std::string> random;
    std::vector<std::string> reverseSorted;
    std::vector<std::string> nearlySorted;
};

Datasets makeDatasets(unsigned int seed) {
    StringGenerator gen(seed);
    Datasets d;
    d.random = gen.generateRandom(MAX_N);
    d.reverseSorted = gen.generateReverseSorted(MAX_N);
    d.nearlySorted = gen.generateNearlySorted(MAX_N, 20);
    return d;
}

const std::vector<std::string>& pickDataset(const Datasets& d, ArrayType t) {
    switch (t) {
        case ArrayType::Random: return d.random;
        case ArrayType::ReverseSorted: return d.reverseSorted;
        case ArrayType::NearlySorted: return d.nearlySorted;
    }
    return d.random;
}

int main() {
    std::cout << "=== String sort empirical study ===\n";

    Datasets data = makeDatasets(42);

    std::vector<Algorithm> algos = {
        {"std_quicksort",   quickSortStandard},
        {"std_mergesort",   mergeSortStandard},
        {"string_quicksort", stringQuickSort3way},
        {"string_mergesort_lcp", stringMergeSortLCP},
        {"msd_radix",       msdRadixSort},
        {"msd_radix_quick", msdRadixSortWithQuick},
    };

    std::vector<ArrayType> types = {
        ArrayType::Random, ArrayType::ReverseSorted, ArrayType::NearlySorted
    };

    std::ofstream out("results/measurements.csv");
    out << "type,algorithm,n,time_us,char_comparisons\n";

    StringSortTester tester;

    for (ArrayType t : types) {
        const auto& full = pickDataset(data, t);
        std::cout << "\n--- array type: " << arrayTypeName(t) << " ---\n";
        for (int n = STEP; n <= MAX_N; n += STEP) {
            std::vector<std::string> subset(full.begin(), full.begin() + n);

            for (const auto& alg : algos) {
                auto res = tester.test(subset, alg.fn, RUNS);

                std::vector<std::string> check = subset;
                alg.fn(check);
                bool ok = StringSortTester::isSorted(check);

                out << arrayTypeName(t) << ","
                    << alg.name << ","
                    << n << ","
                    << std::fixed << std::setprecision(2) << res.timeMicros << ","
                    << res.charComparisons << "\n";

                if (!ok) {
                    std::cerr << "[ERROR] " << alg.name
                              << " did not sort correctly on n=" << n
                              << " type=" << arrayTypeName(t) << "\n";
                }
            }
            std::cout << "n=" << n << " done\n";
        }
    }

    out.close();
    std::cout << "\nResults written to results/measurements.csv\n";
    return 0;
}
