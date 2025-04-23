// Algorithms and/or Data Structures used in this solution:
// Suffix Array (https://cp-algorithms.com/string/suffix-array.html)
// Longest Common Prefix (LCP) (https://cp-algorithms.com/string/suffix-array.html#longest-common-prefix-of-two-substrings-without-additional-memory)

#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

ull gcd(ull a, ull b) {
  if (a == 0) return b;
  return gcd(b % a, a);
}

vector<int> sort_cyclic_shifts(vector<int> &arr) {
  int n = arr.size();
  vector<int> p(n); // permutation array
  vector<int> c(n); // class (or rank) array

  for (int i = 0; i < n; i++) {
    p[i] = i;
    c[i] = arr[i];
  }

  int classes = n;  // number of different classes (ranks)
  vector<int> pn(n), cn(n);

  for (int k = 0; k < n; k ? k *= 2 : k++) {
    // Sort by second half
    for (int i = 0; i < n; i++) {
      pn[i] = (p[i] - k + n) % n;
    }

    // Sort by first half with counting sort
    vector<int> cnt(classes, 0);
    for (int i = 0; i < n; i++) {
      cnt[c[pn[i]]]++;
    }
    for (int i = 1; i < classes; i++) {
      cnt[i] += cnt[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
      p[--cnt[c[pn[i]]]] = pn[i];
    }

    // Compute new classes values
    cn[p[0]] = 0;
    classes = 1;
    for (int i = 1; i < n; i++) {
      pair<int, int> cur = {c[p[i]], c[(p[i] + k) % n]};
      pair<int, int> prev = {c[p[i - 1]], c[(p[i - 1] + k) % n]};
      if (cur != prev) classes++;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }

  return p;
}

vector<int> build_suffix_array(vector<int> &arr) {
  arr.push_back(0);
  vector<int> sorted_shifts = sort_cyclic_shifts(arr);
  arr.pop_back();
  return vector<int>(sorted_shifts.begin() + 1, sorted_shifts.end());
}

vector<int> build_lcp_array(vector<int> &arr, vector<int> &sa) {
  int n = arr.size();
  vector<int> pos(n);  // position of a suffix in the sorted list of suffixes.
  for (int i = 0; i < n; i++) {
    pos[sa[i]] = i;
  }

  vector<int> lcp(n - 1);
  // Each iteration calculates the lcp between the suffix starting at index i and the suffix starting at index j.
  for (int i = 0, k = 0; i < n; i++) {
    if (pos[i] == n - 1) {
      k = 0;
      continue;
    }

    int j = sa[pos[i] + 1];
    while (i + k < n && j + k < n && arr[i + k] == arr[j + k]) {
      k++;
    }
    lcp[pos[i]] = k;

    if (k) k--; // The next lcp value is at least k - 1
  }

  return lcp;
}

// Computes the sum of LCP values over all pairs of suffixes in O(n) time.
// For each pair of suffixes (i, j) with i < j, the LCP value is defined as
// the minimum LCP value in the interval [i..j-1]. We want to compute the sum of
// all such minimums across all valid pairs.
//
// The key idea is to consider each LCP[k] as the minimum of some interval [i, j)
// and compute how many such intervals have LCP[k] as their minimum.
// This can be done by determining how far to the left and right we can extend
// from index k while all values remain >= LCP[k].
ull sum_all_lcps(vector<int> &lcp) {
  int n = lcp.size();

  // choicesLeft[k] = number of indices i such that
  //    i <= k and for all x in [i, k], LCP[x] >= LCP[k]
  // choicesRight[k] = number of indices j such that
  //    k < j and for all x in (k, j), LCP[x] > LCP[k]
  vector<int> choicesLeft(n), choicesRight(n);

  stack<int> st;  // Monotonic stack

  // Compute choices to the left of each position k.
  for (int k = 0; k < n; k++) {
    while (!st.empty() && lcp[st.top()] >= lcp[k]) {
      st.pop();
    }
    choicesLeft[k] = k - (st.empty() ? -1 : st.top());
    st.push(k);
  }

  st = stack<int>();

  // Compute choices to the right of each position k.
  for (int k = n - 1; k >= 0; k--) {
    while (!st.empty() && lcp[st.top()] > lcp[k]) {
      st.pop();
    }
    choicesRight[k] = (st.empty() ? n : st.top()) - k;
    st.push(k);
  }

  // Note that we chose to use '>=' in the left pass and '>' in the right pass to ensure
  // each interval where LCP[k] is the minimum is counted exactly once.
  //
  // This convention avoids double-counting the same subinterval for the same value.
  // However, we could reverse the comparison operators (and the corresponding logic)
  // and still get the same final result. In that case, we would write:
  //
  // choicesLeft[k] = number of possible indices i such that
  //    i <= k and for all x in [i, k), LCP[x] > LCP[k]
  //
  // choicesRight[k] = number of possible indices j such that
  //    k < j and for all x in [k, j), LCP[x] >= LCP[k]
  //
  // This is simply a matter of convention and symmetry: whether we consider LCP[k]
  // to "own" equal values on the left or on the right does not affect correctness:
  // as long as we are consistent we avoid overlapping intervals successfully.

  // Sum up contributions:
  // Each LCP[k] contributes to (choicesLeft[k] * choicesRight[k]) intervals
  // where it is the minimum, so we multiply and accumulate.
  ull sum = 0;
  for (int k = 0; k < n; k++) {
    sum += 1ULL * lcp[k] * choicesLeft[k] * choicesRight[k];
  }

  return sum;
}

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  ull N;
  cin >> N;

  vector<int> moves(N);
  for (auto &v : moves) {
    cin >> v;
  }

  vector<int> sa = build_suffix_array(moves);
  vector<int> lcp = build_lcp_array(moves, sa);

  // 2 * sum_all_lcps(lcp): the sum of LCPs for all pairs (i < j) and (j > i), since similarity is symmetric
  // (N + 1) * N / 2: the sum of lengths of all suffixes (i == j), since each suffix matches itself fully
  ull numerator = 2 * sum_all_lcps(lcp) + (N + 1) * N / 2;
  ull denominator = N * N;

  ull d = gcd(numerator, denominator);
  cout << numerator / d << "/" <<  denominator / d << endl;

  return 0;
}
