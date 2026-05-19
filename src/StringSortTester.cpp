#include "StringSortTester.h"
#include "sorts.h"

#include <algorithm>
#include <chrono>

StringSortTester::Result StringSortTester::test(
    const std::vector<std::string>& input,
    std::function<void(std::vector<std::string>&)> sortFn,
    int runs)
{
    Result r;
    r.timeMicros = 0.0;
    r.charComparisons = 0;

    using clock = std::chrono::steady_clock;

    std::vector<double> times;
    times.reserve(runs);
    uint64_t lastCmps = 0;

    for (int i = 0; i < runs; i++) {
        std::vector<std::string> copy = input;
        resetCharComparisons();
        auto t1 = clock::now();
        sortFn(copy);
        auto t2 = clock::now();
        double micros = std::chrono::duration<double, std::micro>(t2 - t1).count();
        times.push_back(micros);
        lastCmps = getCharComparisons();
    }

    std::sort(times.begin(), times.end());
    r.timeMicros = times[times.size() / 2];
    r.charComparisons = lastCmps;
    return r;
}

bool StringSortTester::isSorted(const std::vector<std::string>& a) {
    for (size_t i = 1; i < a.size(); i++) {
        if (a[i - 1] > a[i]) return false;
    }
    return true;
}
