#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int lcpCompare(const string& a, const string& b, int h, int& outLcp) {
    int n = (int)min(a.size(), b.size());
    while (h < n) {
        if (a[h] != b[h]) {
            outLcp = h;
            return static_cast<unsigned char>(a[h]) < static_cast<unsigned char>(b[h]) ? -1 : 1;
        }
        h++;
    }
    outLcp = h;
    if (a.size() == b.size()) return 0;
    return a.size() < b.size() ? -1 : 1;
}

void merge(vector<string>& a, vector<int>& lcp, int lo, int mid, int hi) {
    int n = hi - lo + 1;
    vector<string> aux(n);
    vector<int> auxLcp(n);
    for (int k = 0; k < n; k++) {
        aux[k] = a[lo + k];
        auxLcp[k] = lcp[lo + k];
    }
    int leftEnd = mid - lo + 1;
    auxLcp[0] = 0;
    auxLcp[leftEnd] = 0;

    int i = 0, j = leftEnd, k = lo;
    while (i < leftEnd && j < n) {
        int li = auxLcp[i], lj = auxLcp[j];
        int cmp, newLcp;
        if (li > lj) { cmp = -1; newLcp = lj; }
        else if (li < lj) { cmp = 1; newLcp = li; }
        else cmp = lcpCompare(aux[i], aux[j], li, newLcp);

        if (cmp <= 0) {
            a[k] = aux[i]; lcp[k] = auxLcp[i];
            auxLcp[j] = newLcp;
            i++;
        } else {
            a[k] = aux[j]; lcp[k] = auxLcp[j];
            auxLcp[i] = newLcp;
            j++;
        }
        k++;
    }
    while (i < leftEnd) { a[k] = aux[i]; lcp[k] = auxLcp[i]; i++; k++; }
    while (j < n)       { a[k] = aux[j]; lcp[k] = auxLcp[j]; j++; k++; }
}

void mergeSortRec(vector<string>& a, vector<int>& lcp, int lo, int hi) {
    if (hi <= lo) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortRec(a, lcp, lo, mid);
    mergeSortRec(a, lcp, mid + 1, hi);
    merge(a, lcp, lo, mid, hi);
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
        vector<int> lcp(n, 0);
        mergeSortRec(a, lcp, 0, n - 1);
    }
    for (const auto& s : a) cout << s << '\n';
    return 0;
}
