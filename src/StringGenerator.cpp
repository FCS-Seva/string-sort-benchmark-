#include "StringGenerator.h"
#include <algorithm>

const std::string StringGenerator::ALPHABET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-.";

StringGenerator::StringGenerator(unsigned int seed) : rng(seed) {}

std::string StringGenerator::randomString() {
    std::uniform_int_distribution<int> lenDist(MIN_LEN, MAX_LEN);
    int len = lenDist(rng);
    return randomString(len);
}

std::string StringGenerator::randomString(int len) {
    std::string s;
    s.reserve(len);
    std::uniform_int_distribution<int> charDist(0, ALPHABET.size() - 1);
    for (int i = 0; i < len; i++) {
        s += ALPHABET[charDist(rng)];
    }
    return s;
}

std::vector<std::string> StringGenerator::generateRandom(int n) {
    std::vector<std::string> result;
    result.reserve(n);
    for (int i = 0; i < n; i++) {
        result.push_back(randomString());
    }
    return result;
}

std::vector<std::string> StringGenerator::generateReverseSorted(int n) {
    std::vector<std::string> result = generateRandom(n);
    std::sort(result.begin(), result.end(), std::greater<std::string>());
    return result;
}

std::vector<std::string> StringGenerator::generateNearlySorted(int n, int swaps) {
    std::vector<std::string> result = generateRandom(n);
    std::sort(result.begin(), result.end());

    if (n < 2) return result;
    std::uniform_int_distribution<int> idxDist(0, n - 1);
    for (int i = 0; i < swaps; i++) {
        int a = idxDist(rng);
        int b = idxDist(rng);
        std::swap(result[a], result[b]);
    }
    return result;
}
