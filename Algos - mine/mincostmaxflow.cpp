/**
 *  Algorithm: Min-cost max-flow
 *  Complexity: Using only Bellman-Ford SPFA: O(|V|^2 * |E|^2)
 *              Using SPFA + Dijkstra with Johnson's Implementation: O(|V| * |E|^2 * log(|V|))
 */
struct Graph {
  vector<vector<int>> adj;
  vector<vector<int>> capacity, cost;
  vector<bool> marked;
  vector<int> parent;
  vector<int> bellman_distance, new_bellman_distance, distance;
  int n;
  i64 max_flow, mincost_maxflow;

  Graph(int _n = 0) {
    init(_n);
  }

  void init(int _n) {
    n = _n;
    adj.resize(n + 1);
    cost.assign(n + 1, vector<int>(n + 1, 0));
    capacity.assign(n + 1, vector<int>(n + 1, 0));
    parent.assign(n + 1, -1);
    bellman_distance.assign(n + 1, INF);
    new_bellman_distance.assign(n + 1, INF);
    distance.assign(n + 1, INF);
  }

  void addEdge(int u, int v, int cs, int cap) {
    adj[u].push_back(v);
    adj[v].push_back(u);
    cost[u][v] = cs;
    cost[v][u] = -cs;
    capacity[u][v] = cap;
  }

  void BellmanFordSPFA(int s, int d) {
    queue<int> q;
    vector<bool> in_queue(n + 1, false);
    q.push(s);
    in_queue[s] = true;
    bellman_distance[s] = 0;
    while (!q.empty()) {
      int cur = q.front();
      q.pop();
      in_queue[cur] = false;
      for (auto next: adj[cur]) {
        if (capacity[cur][next] && bellman_distance[next] > bellman_distance[cur] + cost[cur][next]) {
          bellman_distance[next] = bellman_distance[cur] + cost[cur][next];
          if (!in_queue[next]) {
            q.push(next);
            in_queue[next] = true;
          }
        }
      }
    }
  }

  /**
   * This is a modification of the original Dijkstra algorithm to eliminate the negative weights
   * The idea behind this implementation is that the distance to a node v from node u is changed to
   * mod_cost[u -> v] = cost[u -> u] + pi[v] - pi[u], where pi[x] represents the real distance from
   * the initial node to node x. In terms of minimal costs, pi can be found initially with Bellman-Ford
   * and then updated in the Dijsktra's body.
   * In my implementation:
   *    bellman_distance[x] = pi[x]
   *    new_bellman_distance[x] = The new pi for the graph after augmenting paths
   *    distance[x] = The distance to node x with mod_costs
   */
  bool JohnsonsDijkstra(int s, int d) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, s});
    distance.assign(n + 1, INF);
    new_bellman_distance.assign(n + 1, INF);
    distance[s] = 0;
    new_bellman_distance[s] = 0;

    while (!pq.empty()) {
      auto current = pq.top();
      pq.pop();

      int cur = current.second;
      int64_t cst = current.first;
      if (distance[cur] != cst) continue;

      for (auto next: adj[cur]) {
        if (capacity[cur][next]) {
          if (distance[next] > distance[cur] + cost[cur][next] + bellman_distance[cur] - bellman_distance[next]) {
            distance[next] = distance[cur] + cost[cur][next] + bellman_distance[cur] - bellman_distance[next];
            parent[next] = cur;
            new_bellman_distance[next] = new_bellman_distance[cur] + cost[cur][next];
            pq.push({distance[next], next});
          }
        }
      }
    }

    /// This is the end case. If we haven't reached the sink, then there are no more augmenting paths
    if (distance[d] == INF) return false;

    /// Else, let's compute the flow for the best path (the cost of the path will be stored into
    /// as the value of new_bellman_distance[d]
    i64 current_flow = INF, current_cost = new_bellman_distance[d];

    int path = d;
    while (path != s) {
      current_flow = min(current_flow, 1LL * capacity[parent[path]][path]);
      path = parent[path];
    }

    max_flow += current_flow;
    mincost_maxflow += 1LL * current_flow * current_cost;
    path = d;
    while (path != s) {
      capacity[parent[path]][path] -= current_flow;
      capacity[path][parent[path]] += current_flow;
      path = parent[path];
    }

    /// Update the bellman_distance
    bellman_distance = new_bellman_distance;

    return true;
  }

  void min_cost_max_flow(int s, int d) {
    max_flow = 0;
    mincost_maxflow = 0;
    BellmanFordSPFA(s, d);
    while (JohnsonsDijkstra(s, d));
  }
};
