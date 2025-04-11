// Algorithms used in this solution:
// BFS of a disconnected graph (https://www.geeksforgeeks.org/breadth-first-search-or-bfs-for-a-graph)
// LCA with binary lifting (https://www.baeldung.com/cs/tree-lowest-common-ancestor)

#include <bits/stdc++.h>

using namespace std;

typedef struct tree_node {
  int value;
  int edge = 0;
  int parent = 0;
  int depth = 0;
  int component;
} tree_node;

typedef struct graph_node {
  int value;
  int edge;
} graph_node;

int main(void) {
  ios::sync_with_stdio(false);
  cin.tie(NULL);

  int N;  // Number of movies
  int M;  // Number of actors
  cin >> N >> M;

  vector<vector<int>> movie_actors(N + 1, vector<int>());
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

  vector<vector<graph_node>> graph(N + 1);

  for (int movie1 = 1; movie1 <= N; movie1++) {
    for (int movie2 = movie1 + 1; movie2 <= N; movie2++) {
      int actor = find_common_actor(movie1, movie2);
      if (!actor) continue;
      graph[movie1].push_back({ movie2, actor });
      graph[movie2].push_back({ movie1, actor });
    }
  }

  vector<tree_node> spanning_tree(N + 1);
  vector<bool> visited(N + 1);

  // BFS
  for (int comp = 1, m = 1; m <= N; m++) {
    if (visited[m]) continue;

    queue<int> q;
    q.push(m);
    visited[m] = true;
    spanning_tree[m] = {
      .value = m,
      .component = comp,
    };

    while (!q.empty()) {
      int movie = q.front();
      q.pop();

      for (graph_node node : graph[movie]) {
        if (visited[node.value]) continue;
        q.push(node.value);
        visited[node.value] = true;
        spanning_tree[node.value] = {
          .value = node.value,
          .edge = node.edge,
          .parent = movie,
          .depth = spanning_tree[movie].depth + 1,
          .component = comp,
        };
      }
    }
    comp++;
  }

  // Binary Lifting Sparse Table
  int logN = ceil(log2(N));
  vector<vector<int>> ST(N + 1, vector<int>(logN + 1, 0));
  for (int i = 1; i <= N; i++) {
    ST[i][0] = spanning_tree[i].parent;
  }
  for (int j = 1; j <= logN; j++) {
    for (int i = 1; i <= N; i++) {
      int k = ST[i][j - 1];
      ST[i][j] = ST[k][j - 1];
    }
  }

  auto lca = [&](tree_node u, tree_node v) {
    if (u.depth < v.depth) swap(u, v);

    // Adjust both nodes to the same depth
    int depth_diff = u.depth - v.depth;
    for (int i = logN; i >= 0; i--) {
      if ((1 << i) > depth_diff) continue;
      u = spanning_tree[ST[u.value][i]];
      depth_diff -= (1 << i);
    }

    if (u.value == v.value) return u;

    // Find lca
    for (int i = logN; i >= 0; i--) {
      if (ST[u.value][i] == ST[v.value][i]) continue;
      u = spanning_tree[ST[u.value][i]];
      v = spanning_tree[ST[v.value][i]];
    }

    return spanning_tree[ST[u.value][0]];
  };

  auto solve = [&](int actor1, int actor2){
    for (int movie1 = 1; movie1 <= N; movie1++) {
      if (!has[movie1][actor1]) continue;
      for (int movie2 = 1; movie2 <= N; movie2++) {
        if (!has[movie2][actor2]) continue;

        tree_node u = spanning_tree[movie1];
        tree_node v = spanning_tree[movie2];

        if (u.component != v.component) continue;

        tree_node r = lca(u, v);
        vector<int> u2r, v2r;

        u2r.push_back(actor1);
        for (auto n = u; n.value != r.value; n = spanning_tree[n.parent]) {
          u2r.push_back(n.value);
          u2r.push_back(n.edge);
        }

        v2r.push_back(actor2);
        for (auto n = v; n.value != r.value; n = spanning_tree[n.parent]) {
          v2r.push_back(n.value);
          v2r.push_back(n.edge);
        }

        cout << (u2r.size() + v2r.size() + 2) / 2 << endl;
        for (auto &x : u2r){
          cout << x << " ";
        }
        cout << r.value << " ";
        for (auto it = v2r.rbegin(); it != v2r.rend(); it++) {
          cout << *it << " ";
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
