#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

inline int charAt(const string& s, int d) {
    return d < (int)s.size() ? (unsigned char)s[d] : -1;
}

int medianOfThreeChar(vector<string>& a, int i, int j, int k, int d) {
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

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    cin.ignore();
    vector<string> a(n);
    for (int i = 0; i < n; i++) getline(cin, a[i]);

    if (n >= 2) stringQuickSort(a, 0, n - 1, 0);
    for (const auto& s : a) cout << s << '\n';
    return 0;
}
