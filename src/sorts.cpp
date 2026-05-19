#include "sorts.h"

#include <algorithm>
#include <vector>
#include <string>

uint64_t charComparisons = 0;

void resetCharComparisons() { charComparisons = 0; }
uint64_t getCharComparisons() { return charComparisons; }

static const int ALPHABET_SIZE = 74;
static const int RADIX = 256;


static int compareStrings(const std::string& a, const std::string& b) {
    int n = std::min((int)a.size(), (int)b.size());
    for (int i = 0; i < n; i++) {
        ++charComparisons;
        if (a[i] != b[i]) {
            return (unsigned char)a[i] < (unsigned char)b[i] ? -1 : 1;
        }
    }
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
}

static int lcpCompare(const std::string& a, const std::string& b, int h, int& outLcp) {
    int n = std::min((int)a.size(), (int)b.size());
    while (h < n) {
        ++charComparisons;
        if (a[h] != b[h]) {
            outLcp = h;
            return (unsigned char)a[h] < (unsigned char)b[h] ? -1 : 1;
        }
        h++;
    }
    outLcp = h;
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
}

static inline int charAt(const std::string& s, int d) {
    if (d < (int)s.size()) {
        ++charComparisons;
        return (unsigned char)s[d];
    }
    return -1;
}

static int medianOfThreeFull(std::vector<std::string>& a, int i, int j, int k) {
    int cij = compareStrings(a[i], a[j]);
    int cik = compareStrings(a[i], a[k]);
    int cjk = compareStrings(a[j], a[k]);
    if (cij <= 0) {
        if (cjk <= 0) return j;
        if (cik <= 0) return k;
        return i;
    } else {
        if (cjk > 0) return j;
        if (cik > 0) return k;
        return i;
    }
}

static int medianOfThreeChar(std::vector<std::string>& a, int i, int j, int k, int d) {
    int ci = charAt(a[i], d);
    int cj = charAt(a[j], d);
    int ck = charAt(a[k], d);
    if (ci <= cj) {
        if (cj <= ck) return j;
        if (ci <= ck) return k;
        return i;
    } else {
        if (cj > ck) return j;
        if (ci > ck) return k;
        return i;
    }
}


static void quickSortStandardRec(std::vector<std::string>& a, int lo, int hi) {
    if (hi <= lo) return;

    // pivot = медиана из трех
    int mid = lo + (hi - lo) / 2;
    int pivotIdx = medianOfThreeFull(a, lo, mid, hi);
    std::swap(a[pivotIdx], a[hi]);
    std::string pivot = a[hi];

    // схема Lomuto
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (compareStrings(a[j], pivot) <= 0) {
            i++;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[hi]);
    int p = i + 1;

    quickSortStandardRec(a, lo, p - 1);
    quickSortStandardRec(a, p + 1, hi);
}

void quickSortStandard(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    quickSortStandardRec(a, 0, (int)a.size() - 1);
}


static void mergeStandard(std::vector<std::string>& a, int lo, int mid, int hi) {
    std::vector<std::string> aux(a.begin() + lo, a.begin() + hi + 1);
    int n_left = mid - lo + 1;
    int total = hi - lo + 1;
    int i = 0, j = n_left, k = lo;

    while (i < n_left && j < total) {
        if (compareStrings(aux[i], aux[j]) <= 0) {
            a[k++] = aux[i++];
        } else {
            a[k++] = aux[j++];
        }
    }
    while (i < n_left) a[k++] = aux[i++];
    while (j < total) a[k++] = aux[j++];
}

static void mergeSortStandardRec(std::vector<std::string>& a, int lo, int hi) {
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortStandardRec(a, lo, mid);
    mergeSortStandardRec(a, mid + 1, hi);
    mergeStandard(a, lo, mid, hi);
}

void mergeSortStandard(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    mergeSortStandardRec(a, 0, (int)a.size() - 1);
}

static void stringQuickSort3wayRec(std::vector<std::string>& a, int lo, int hi, int d) {
    if (hi <= lo) return;

    int mid = lo + (hi - lo) / 2;
    int pivotIdx = medianOfThreeChar(a, lo, mid, hi, d);
    std::swap(a[pivotIdx], a[lo]);
    int pivotChar = charAt(a[lo], d);

    int lt = lo;
    int gt = hi;
    int i = lo + 1;
    while (i <= gt) {
        int t = charAt(a[i], d);
        if (t < pivotChar) {
            std::swap(a[lt], a[i]);
            lt++;
            i++;
        } else if (t > pivotChar) {
            std::swap(a[i], a[gt]);
            gt--;
        } else {
            i++;
        }
    }
    stringQuickSort3wayRec(a, lo, lt - 1, d);
    if (pivotChar >= 0) {
        stringQuickSort3wayRec(a, lt, gt, d + 1);
    }
    stringQuickSort3wayRec(a, gt + 1, hi, d);
}

void stringQuickSort3way(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    stringQuickSort3wayRec(a, 0, (int)a.size() - 1, 0);
}

static void mergeWithLCP(std::vector<std::string>& a, std::vector<int>& lcp,
                         int lo, int mid, int hi)
{
    int n = hi - lo + 1;
    std::vector<std::string> aux(n);
    std::vector<int> auxLcp(n);
    for (int k = 0; k < n; k++) {
        aux[k] = a[lo + k];
        auxLcp[k] = lcp[lo + k];
    }
    int leftEnd = mid - lo + 1;
    auxLcp[0] = 0;
    auxLcp[leftEnd] = 0;

    int i = 0;
    int j = leftEnd;
    int k = lo;

    while (i < leftEnd && j < n) {
        int li = auxLcp[i];
        int lj = auxLcp[j];

        int cmp;
        int newLcp;

        if (li > lj) {
            cmp = -1;
            newLcp = lj;
        } else if (li < lj) {
            cmp = 1;
            newLcp = li;
        } else {
            cmp = lcpCompare(aux[i], aux[j], li, newLcp);
        }

        if (cmp <= 0) {
            a[k] = aux[i];
            lcp[k] = auxLcp[i];
            auxLcp[j] = newLcp;
            i++;
        } else {
            a[k] = aux[j];
            lcp[k] = auxLcp[j];
            auxLcp[i] = newLcp;
            j++;
        }
        k++;
    }
    while (i < leftEnd) {
        a[k] = aux[i];
        lcp[k] = auxLcp[i];
        i++; k++;
    }
    while (j < n) {
        a[k] = aux[j];
        lcp[k] = auxLcp[j];
        j++; k++;
    }
}

static void stringMergeSortLCPRec(std::vector<std::string>& a, std::vector<int>& lcp,
                                  int lo, int hi)
{
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2;
    stringMergeSortLCPRec(a, lcp, lo, mid);
    stringMergeSortLCPRec(a, lcp, mid + 1, hi);
    mergeWithLCP(a, lcp, lo, mid, hi);
}

void stringMergeSortLCP(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    std::vector<int> lcp(a.size(), 0);
    stringMergeSortLCPRec(a, lcp, 0, (int)a.size() - 1);
}


static void msdRadixRec(std::vector<std::string>& a, std::vector<std::string>& aux,
                        int lo, int hi, int d, bool useQuickFallback) {
    if (hi <= lo) return;

    if (useQuickFallback && (hi - lo + 1) < ALPHABET_SIZE) {
        stringQuickSort3wayRec(a, lo, hi, d);
        return;
    }

    std::vector<int> count(RADIX + 2, 0);

    for (int i = lo; i <= hi; i++) {
        int c = charAt(a[i], d);
        count[c + 2]++;
    }
    for (int r = 0; r < RADIX + 1; r++) {
        count[r + 1] += count[r];
    }
    std::vector<int> bucketStart(count);

    for (int i = lo; i <= hi; i++) {
        int c = charAt(a[i], d);
        aux[lo + count[c + 1]++] = a[i];
    }
    for (int i = lo; i <= hi; i++) {
        a[i] = aux[i];
    }

    for (int r = 1; r < RADIX + 1; r++) {
        int subLo = lo + bucketStart[r];
        int subHi = lo + bucketStart[r + 1] - 1;
        if (subHi > subLo) {
            msdRadixRec(a, aux, subLo, subHi, d + 1, useQuickFallback);
        }
    }
}

void msdRadixSort(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    std::vector<std::string> aux(a.size());
    msdRadixRec(a, aux, 0, (int)a.size() - 1, 0, false);
}

void msdRadixSortWithQuick(std::vector<std::string>& a) {
    if (a.size() < 2) return;
    std::vector<std::string> aux(a.size());
    msdRadixRec(a, aux, 0, (int)a.size() - 1, 0, true);
}
