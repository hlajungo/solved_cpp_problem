#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

template<typename V>
class Graph {
  public:
    void add_edge(const V &from, const V &to);
    void add_vertex(const V &vertex);
    const std::unordered_set<V> get_children(const V &vertex) const;
    const std::unordered_set<V> get_parents(const V &vertex) const;
    const std::unordered_set<V> get_neighbours(const V &vertex) const;
    const std::unordered_set<V> get_descendants(const V &vertex) const;
    const std::unordered_set<V> get_ancestors(const V &vertex) const;

    struct Vertex {
      std::unordered_set<V> parents;
      std::unordered_set<V> children;
    };
    std::unordered_map<V, Vertex> _edges;
};


const int INF = numeric_limits<int>::max();


int dijkstra(int from, int to, int vertex[], int edge[7][7])
{
    vector<int> dist(7, INF); // 從 from 到其他點的距離
    priority_queue<pair<int, int>, vector<pair<int, int>>
      ,greater<pair<int, int>>> pq; // pq<dist, vertex>, 一條到達點的路，權重為 dist。
    dist[from] = 0; // 到自己
    pq.emplace(0, from); // 到自己

}
int dijkstra(int from, int to, int vertex[], int edge[7][7])
{
    vector<int> dist(7, INF);
    
    // 由於使用最小推，需要提供儲存空間vector<pair<int, int>>, 若為最大推則不用。
    // 這東西只能訪問 top()，並保證最大/最小，使用 pair 第一個參數排序
    // pq <distance, vertex>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[from] = 0;
    // pq.emplace(std::make_pair(0, from));
    // pq 會調用 pair(T1&& t1, T2&& t2);
    pq.emplace(0, from);

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // 目前有更好的，或者說，算過了
        if (d > dist[u]) continue;

        for (int v = 0; v < 7; ++v)
        {
            if (edge[u][v] > 0)
            { // 有邊存在
                int newDist = dist[u] + edge[u][v];
                if (newDist < dist[v])
                {
                    dist[v] = newDist;
                    pq.emplace(newDist, v);
                }
            }
        }
    }

    return (dist[to] == INF ? -1 : dist[to]);
}


int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int edge[7][7] =
  {
    {0,0,1,0,1,0,1},
    {0,0,0,1,1,0,0},
    {1,0,0,1,0,1,1},
    {0,1,1,0,0,1,0},
    {1,1,0,0,0,0,0},
    {0,0,1,1,0,0,1},
    {1,0,1,0,0,1,0}
  };
  int vertex[7] = {0, 1, 2, 3, 4, 5, 6};
  int from, to;
  while (cin >> from >> to)
  {
    if (from == 0 && to == 0)
      return 0;

    cout << dijkstra (from, to, vertex, edge) << endl;
  }

}
