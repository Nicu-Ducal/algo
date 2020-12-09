/// Undirected/directed graph (for directed delete a push_back from the addEdge
struct Graph {
  vector<vector<int>> adj;
  vector<bool> marked;
  vector<int> parent, level;
  int n;

  Graph(int _n = 0) {
    init(_n);
  }

  void init(int _n) {
    n = _n;
    adj.resize(n + 1);
    marked.resize(n + 1);
  }

  void addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
};

/// Weighted graph
struct Graph {
  vector<vector<pair<int, int>>> adj;
  vector<bool> marked;
  vector<int> parent, level;
  int n;

  Graph(int _n = 0) {
    init(_n);
  }

  void init(int _n) {
    n = _n;
    adj.resize(n + 1);
    marked.resize(n + 1);
  }

  void addEdge(int u, int v, int w) {
    adj[u].push_back({v, w});
    adj[v].push_back({u, w});
  }
};
