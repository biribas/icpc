#include <bits/stdc++.h>
#include <vector>

using namespace std;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N, sum = 0;
  cin >> N;

  vector<int> a(N);
  for (auto &v : a) {
    cin >> v;
    sum += v;
  }

  if (sum & 1) {
    cout << -1 << endl;
    return 0;
  }

  const int W = sum / 2;
  vector<vector<bool>> dp(N + 1, vector<bool>(W + 1, false));
  dp[0][0] = true;

  for (int i = 1; i <= N; i++) {
    int w = a[i - 1];
    for (int j = 0; j <= W; j++) {
      if (w > j)
        dp[i][j] = dp[i - 1][j];
      else
        dp[i][j] = dp[i - 1][j] || dp[i - 1][j - w];
    }
  }

  if (dp[N][W] == false) {
    cout << -1 << endl;
    return 0;
  }

  stack<int> alice_knapsack, bob_knapsack;

  int i = N;
  for (int j = W; i > 0 && j >= 0; i--) {
    if (j == 0 || dp[i][j] == dp[i - 1][j]) {
      bob_knapsack.push(a[i - 1]);
    }
    else {
      j -= a[i - 1];
      alice_knapsack.push(a[i - 1]);
    }
  }

  int alice = 0, bob = 0;
  while (!(alice_knapsack.empty() && bob_knapsack.empty())) {
    if (alice <= bob) {
      cout << alice_knapsack.top() << " ";
      alice += alice_knapsack.top();
      alice_knapsack.pop();
    } else {
      cout << bob_knapsack.top() << " ";
      bob += bob_knapsack.top();
      bob_knapsack.pop();
    }
  }

  cout << endl;
  return 0;
}
