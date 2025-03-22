#include <bits/stdc++.h>

using namespace std;

typedef struct {
  string data;
  string guess;
  vector<int> noise_positions;
} message;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  message messages[2];
  message *M = &messages[0];
  message *N = &messages[1];

  cin >> messages[0].data >> messages[1].data;

  for (auto &message : messages) {
    message.guess = message.data;
    for (int i = 0; i < message.data.size(); i++) {
      if (message.data[i] != '*') continue;
      message.noise_positions.push_back(i);
    }
  }

  int total_noise = M->noise_positions.size() + N->noise_positions.size();
  for (int i = 0, n = 1 << total_noise; i < n; i++) {
    int guess = i;
    for (auto &message : messages) {
      for (auto pos : message.noise_positions) {
        int bit = guess & 1;
        message.guess[pos] = bit + '0';
        guess >>= 1;
      }
    }

    // Convert guessed N from string to integer
    int guessed_N = 0;
    for (int i = N->guess.size() - 1, shift = 0; i >= 0; i--, shift++) {
      guessed_N |= (N->guess[i] - '0') << shift;
    }

    // Calculate remainder
    int remainder = 0;
    for (char bit : M->guess) {
      remainder = (2 * remainder + (bit == '1')) % guessed_N;
    }

    if (remainder == 0) {
      cout << M->guess << endl;
      return 0;
    }
  }

  return 0;
}
