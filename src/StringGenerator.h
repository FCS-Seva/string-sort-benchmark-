#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <string>
#include <vector>
#include <random>

class StringGenerator {
public:
    static const std::string ALPHABET;

    static const int MIN_LEN = 10;
    static const int MAX_LEN = 200;
    static const int MAX_SIZE = 3000;

    StringGenerator(unsigned int seed = 42);

    std::vector<std::string> generateRandom(int n);

    std::vector<std::string> generateReverseSorted(int n);

    std::vector<std::string> generateNearlySorted(int n, int swaps = 5);

private:
    std::mt19937 rng;

    std::string randomString();
    std::string randomString(int len);
};

#endif
