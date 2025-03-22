#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;

#define SETUP ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(0);

#define MAX_VALUE 1000000
#define MAX_FOOD 100000
#define MOD 1000000007

int main(void) {
  SETUP

  // spf[n] = smallest prime factor of n
  vector<int> spf(MAX_VALUE + 1, 1);

  for (ull p = 2; p <= MAX_VALUE; p++) {
    if (spf[p] != 1) continue;
    spf[p] = p;

    // After p, the next number n for which spf[n] = p is p * p.
    for (ull i = p * p; i <= MAX_VALUE; i += p) {
      if (spf[i] == 1) spf[i] = p;
    }
  }

  // powers_of_two[n] = 2^n % MOD
  vector<int> powers_of_two(MAX_FOOD + 1, 1);
  for (ull i = 1; i <= MAX_FOOD; i++) {
    powers_of_two[i] = 2 * powers_of_two[i - 1] % MOD;
  }

  int N, V;
  cin >> N;

  // foods[V] = Number of foods associated with value V
  vector<int> foods(MAX_VALUE + 1, 0);
  for (int i = 0; i < N; i++) {
    cin >> V;
    foods[V]++;
  }

  // freq[n] = Number of foods divisible by n
  vector<int> freq(MAX_VALUE + 1, 0);
  for (ull i = 2; i <= MAX_VALUE; i++) {
    for (ull j = i; j <= MAX_VALUE; j += i) {
      freq[i] += foods[j];
    }
  }

  int Q, X;
  cin >> Q;

  while (Q--) {
    cin >> X;
    vector<int> prime_factors;
    while (X > 1) {
      int p = spf[X];
      while (X % p == 0) X /= p;
      prime_factors.push_back(p);
    }

    int forbidden_foods = 0;

    // Inclusion-Exclusion Principle
    for (int i = 1; i < 1 << prime_factors.size(); i++) {   // For each subset of prime factors
      int cur_subset = 1, subset_size = 0;
      for (int j = 0; j < prime_factors.size(); j++) {      // Select prime factors for the subset
        if (i & (1 << j)) {
          cur_subset *= prime_factors[j];
          subset_size++;
        }
      }
      forbidden_foods += freq[cur_subset] * (subset_size & 1 ? 1 : -1);
    }

    cout << powers_of_two[N - forbidden_foods] << endl;
  }

  return 0;
}
