#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long ull;

#define MAX_VALUE 1000000
#define MAX_FOOD 100000
#define MOD 1000000007

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

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

  // Mobius function
  // mobius[n] = 1, if n = 1
  // mobius[n] = 0, if n has a squared prime factor
  // mobius[n] = (-1)^k, if n is a product of k distinct primes factors
  vector<int> mobius(MAX_VALUE, 1);
  for (ull n = 2; n <= MAX_VALUE; n++) {
    int quotient = n / spf[n];
    if (spf[n] == spf[quotient]) {
      mobius[n] = 0;
    } else {
      mobius[n] = -1 * mobius[quotient];
    }
  }

  // forbidden_foods[n] = Number of foods forbidden for an allergy n
  vector<int> forbidden_foods(MAX_VALUE + 1, 0);

  // Inclusion-Exclusion Principle
  for (int i = 2; i <= MAX_VALUE; i++) {
    for (int j = i; j <= MAX_VALUE; j += i) {
      forbidden_foods[j] += -1 * mobius[i] * freq[i];
    }
  }

  int Q, X;
  cin >> Q;

  while (Q--) {
    cin >> X;
    cout << powers_of_two[N - forbidden_foods[X]] << endl;
  }

  return 0;
}
