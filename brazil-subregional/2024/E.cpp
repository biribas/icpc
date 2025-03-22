#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>

using namespace std;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N;
  cin >> N;

  vector<int> corners(4);
  int dummy;

  cin >> corners[0];
  for (int j = 1; j < N - 1; j++) cin >> dummy;
  cin >> corners[1];

  for (int i = 1; i < N - 1; i++)
    for (int j = 0; j < N; j++)
      cin >> dummy;

  cin >> corners[3];
  for (int j = 1; j < N - 1; j++) cin >> dummy;
  cin >> corners[2];

  cout << distance(corners.begin(), min_element(corners.begin(), corners.end())) << endl;
  return 0;
}
