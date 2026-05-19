#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <string>
#include <vector>
#include <functional>
#include <cstdint>

class StringSortTester {
public:
    struct Result {
        double timeMicros;
        uint64_t charComparisons;
    };


    Result test(const std::vector<std::string>& input,
                std::function<void(std::vector<std::string>&)> sortFn,
                int runs = 5);

    static bool isSorted(const std::vector<std::string>& a);
};

#endif
