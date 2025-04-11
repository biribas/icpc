#include <bits/stdc++.h>

using namespace std;

typedef struct node {
  int value;
  int edge = 0;
} node;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N;  // Number of movies
  int M;  // Number of actors
  cin >> N >> M;

  // movie_actors[movie] = actors in movie
  vector<vector<int>> movie_actors(N + 1, vector<int>());
  // has[movie][actor] = whether actor has appeared in movie
  vector<vector<bool>> has(N + 1, vector<bool>(M + 1, false));

  for (int a, n, m = 1; m <= N; m++) {
    cin >> n;
    while (n--) {
      cin >> a;
      movie_actors[m].push_back(a);
      has[m][a] = true;
    }
  }

  auto find_common_actor = [&](int movie1, int movie2) {
    if (movie_actors[movie1].size() > movie_actors[movie2].size()) {
      swap(movie1, movie2);
    }
    for (auto actor : movie_actors[movie1]) {
      if (has[movie2][actor]) return actor;
    }
    return 0;
  };

  vector<vector<node>> graph(N + 1);
  for (int movie1 = 1; movie1 <= N; movie1++) {
    for (int movie2 = movie1 + 1; movie2 <= N; movie2++) {
      int actor = find_common_actor(movie1, movie2);
      if (!actor) continue;
      graph[movie1].push_back({ movie2, actor });
      graph[movie2].push_back({ movie1, actor });
    }
  }

  // parent[m][j] = parent of j in BFS starting at m
  vector<vector<node>> parent(N + 1, vector<node>(N + 1));
  // visited[m][j] = whether j has been visited in BFS starting at m
  vector<vector<bool>> visited(N + 1, vector<bool>(N + 1, false));
  // Component ID of each movie
  vector<int> component(N + 1, 0);

  // BFS
  for (int comp = 1, m = 1; m <= N; m++) {
    queue<int> q;
    q.push(m);
    visited[m][m] = true;
    component[m] = comp;

    while (!q.empty()) {
      int movie = q.front();
      q.pop();

      for (node n : graph[movie]) {
        if (visited[m][n.value]) continue;
        q.push(n.value);
        parent[m][n.value] = {
          .value = movie,
          .edge = n.edge,
        };
        visited[m][n.value] = true;
        component[n.value] = comp;
      }
    }
    comp++;
  }

  auto solve = [&](int actor1, int actor2){
    for (int movie1 = 1; movie1 <= N; movie1++) {
      if (!has[movie1][actor1]) continue;
      for (int movie2 = 1; movie2 <= N; movie2++) {
        if (!has[movie2][actor2]) continue;
        if (component[movie1] != component[movie2]) continue;

        vector<int> ans;
        ans.push_back(actor2);

        int m = movie2;
        while (true) {
          ans.push_back(m);
          auto node = parent[movie1][m];
          if (!node.value) {
            ans.push_back(actor1);
            break;
          };
          ans.push_back(node.edge);
          m = node.value;
        }

        cout << (ans.size() + 1) / 2 << endl;

        // Print in reverse order
        for (auto it = ans.rbegin(); it != ans.rend(); it++) {
          cout << *it << ' ';
        }
        cout << endl;

        return;
      }
    }
    cout << -1 << endl;
  };

  int Q;
  cin >> Q;
  int actor1, actor2;
  while (Q--) {
    cin >> actor1 >> actor2;
    solve(actor1, actor2);
  }
  return 0;
}
