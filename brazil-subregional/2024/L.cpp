#include <bits/stdc++.h>

using namespace std;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N, max_bit = 0;
  cin >> N;

  vector<stack<int>> table(32);
  vector<int> arr(N);

  for (int i = 0; i < N; i++) {
    cin >> arr[i];

    for (int bit = 0, n = arr[i]; n != 0; bit++, n >>= 1) {
      if ((n & 1) == 0) continue;
      table[bit].push(i);
      max_bit = max(max_bit, bit);
    }
  }

  for (int i = 0; i < N; i++) {
    for (int k = 0, n = arr[i]; k <= max_bit; k++, n >>= 1) {
      if (n & 1) continue;
      if (table[k].size() == 0) continue;

      int j = table[k].top();
      if (j <= i) continue;

      table[k].pop();

      arr[i] ^= 1 << k;
      arr[j] ^= 1 << k;
    }
  }

  for (int i = 0; i < N; i++) {
    cout << arr[i] << " ";
  }
  cout << endl;

  return 0;
}
