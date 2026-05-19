#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <string>
#include <cstdint>

extern uint64_t charComparisons;

void resetCharComparisons();
uint64_t getCharComparisons();

void quickSortStandard(std::vector<std::string>& a);
void mergeSortStandard(std::vector<std::string>& a);

void stringQuickSort3way(std::vector<std::string>& a);

void stringMergeSortLCP(std::vector<std::string>& a);

void msdRadixSort(std::vector<std::string>& a);

void msdRadixSortWithQuick(std::vector<std::string>& a);

#endif
