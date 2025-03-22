#include <bits/stdc++.h>

using namespace std;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N;
  cin >> N;

  vector<int> F(N + 1);
  F[0] = 1;
  F[1] = 1;

  for (int i = 2; i <= N; i++) {
    F[i] = F[i - 1] + F[i - 2];
  }

  cout << F[N] << endl;
  return 0;
}
