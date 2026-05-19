#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

static constexpr int RADIX = 256;
static constexpr int ALPHABET_SIZE = 74;

inline int charAt(const string& s, int d) {
    return d < static_cast<int>(s.size()) ? static_cast<unsigned char>(s[d]) : -1;
}

int medianOfThreeChar(vector<string>& a, int i, int j, int k, int d) {
    const int ci = charAt(a[i], d);
    const int cj = charAt(a[j], d);
    const int ck = charAt(a[k], d);
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

void stringQuickSort(vector<string>& a, int lo, int hi, int d) {
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2;
    int pivotIdx = medianOfThreeChar(a, lo, mid, hi, d);
    swap(a[pivotIdx], a[lo]);
    int pivotChar = charAt(a[lo], d);

    int lt = lo, gt = hi, i = lo + 1;
    while (i <= gt) {
        int t = charAt(a[i], d);
        if (t < pivotChar) { swap(a[lt], a[i]); lt++; i++; }
        else if (t > pivotChar) { swap(a[i], a[gt]); gt--; }
        else i++;
    }
    stringQuickSort(a, lo, lt - 1, d);
    if (pivotChar >= 0) stringQuickSort(a, lt, gt, d + 1);
    stringQuickSort(a, gt + 1, hi, d);
}

void msdRadix(vector<string>& a, vector<string>& aux, int lo, int hi, int d) {
    if (hi <= lo) return;

    if ((hi - lo + 1) < ALPHABET_SIZE) {
        stringQuickSort(a, lo, hi, d);
        return;
    }

    vector<int> count(RADIX + 2, 0);
    for (int i = lo; i <= hi; i++) count[charAt(a[i], d) + 2]++;
    for (int r = 0; r < RADIX + 1; r++) count[r + 1] += count[r];
    const vector<int> bucketStart(count);
    for (int i = lo; i <= hi; i++) {
        int c = charAt(a[i], d);
        aux[lo + count[c + 1]++] = a[i];
    }
    for (int i = lo; i <= hi; i++) a[i] = aux[i];

    for (int r = 1; r < RADIX + 1; r++) {
        const int subLo = lo + bucketStart[r];
        const int subHi = lo + bucketStart[r + 1] - 1;
        if (subHi > subLo) msdRadix(a, aux, subLo, subHi, d + 1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    cin.ignore();
    vector<string> a(n);
    for (int i = 0; i < n; i++) getline(cin, a[i]);

    if (n >= 2) {
        vector<string> aux(n);
        msdRadix(a, aux, 0, n - 1, 0);
    }
    for (const auto& s : a) cout << s << '\n';
    return 0;
}
