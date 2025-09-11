#include <bits/stdc++.h>

using namespace std;
template <typename weight_t = int,
          typename vertex_t = int,
          typename int_t = int>
class Adj_list_unordered_map_weighted
{
private:
  using graph_t
      = std::unordered_map<vertex_t,
                           std::vector<std::pair<weight_t, vertex_t> > >;

  graph_t graph; /// Graph

public:
  Adj_list_unordered_map_weighted () {}

  void
  add_edge (weight_t w, vertex_t u, vertex_t v)
  {
    graph[u].emplace_back (w, v);
    graph[v].emplace_back (w, u);
  }

  void
  add_undirected_edge (weight_t w, vertex_t u, vertex_t v)
  {
    graph[u].emplace_back (w, v);
  }

  graph_t
  get_adj_list () const
  {
    return graph;
  }

  size_t
  get_vertex_num () const
  {
    return graph.size ();
  }

  void
  print () const
  {
    print (this->graph);
  }

  void
  print (const graph_t& graph) const
  {
    std::cout << "[\n";
    for (const auto& u : graph)
    {
      std::cout << "  " << u.first << " : [ ";
      for (const auto& v : u.second)
      {
        std::cout << v.second << " ";
      }
      std::cout << "]\n";
    }
    std::cout << "]\n";
  }

  void
  primMST_graph_weight (const vertex_t& start_v,
                        graph_t& ret_graph,
                        int_t& ret_min_cost)
  {
    // tuple min_heap, tuple = <v_weight, v_vertex, u_vertex>
    std::priority_queue<
        std::tuple<weight_t, vertex_t, vertex_t>,
        std::vector<std::tuple<weight_t, vertex_t, vertex_t> >,
        std::greater<std::tuple<weight_t, vertex_t, vertex_t> > >
        min_heap;

    std::vector<bool> visited (graph.size (), false);
    bool first = true;
    min_heap.push ({ 0, start_v, 0 });
    while (!min_heap.empty ())
    {
      auto [w, u, from_vertex] = min_heap.top ();
      min_heap.pop ();
      if (visited[u])
        continue;
      // 將要選擇此點，並加入他的鄰點
      ret_min_cost += w;
      if (first == false)
      {
        ret_graph[from_vertex].push_back ({ w, u });
        ret_graph[u].push_back ({ w, from_vertex });
      }
      first = false;

      visited[u] = 1;
      for (auto v : graph[u])
      {
        if (visited[v.second])
          continue;
        min_heap.push (std::make_tuple (v.first, v.second, u));
      }
    }
  }

  vector<pair<vertex_t, vertex_t> >
  get_edges () const
  {
    auto ret = get_edges (this->graph);
    return ret;
  }

  vector<pair<vertex_t, vertex_t> >
  get_edges (const graph_t& graph) const
  {
    vector<pair<vertex_t, vertex_t> > ret;

    for (auto& i : graph)
    {
      for (auto& j : i.second)
      {
        ret.push_back ({ i.first, j.second });
      }
    }
    return ret;
  }
};

int
main ()
{
  int n, m;
  cin >> n >> m;
  std::unordered_map<string, int> mapper_str_2_idx;
  std::unordered_map<int, string> mapper_idx_2_str;
  for (int i = 0; i < n; ++i)
  {
    string str;
    cin >> str;
    mapper_str_2_idx[str] = i;
    mapper_idx_2_str[i] = str;
  }
  Adj_list_unordered_map_weighted alumw;
  for (int i = 0; i < m; ++i)
  {
    string str1, str2;
    int w;
    cin >> str1 >> str2 >> w;

    alumw.add_edge (w, mapper_str_2_idx[str1], mapper_str_2_idx[str2]);
  }
  std::unordered_map<int, std::vector<std::pair<int, int> > > ret;
  int min_cost = 0;

  alumw.primMST_graph_weight (0, ret, min_cost);
  auto edges = alumw.get_edges (ret);
  sort (edges.begin (),
        edges.end (),
        [&] (pair<int, int>& a, pair<int, int>& b)
        {
          if (a.first == b.first)
          {
            return a.second < b.second;
          }
          return a.first < b.first;
        });
  // cout
  for (auto& e : edges)
  {
    if (e.first < e.second)
      cout << "(" << mapper_idx_2_str[e.first] << " "
           << mapper_idx_2_str[e.second] << ")";
  }
  cout << '\n';
  cout << min_cost << '\n';
}
