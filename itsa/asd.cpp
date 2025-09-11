#include <algorithm>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;

// Union Find class
class UnionFind
{
public:
  vector<int> parent;

  UnionFind (int n) : parent (n)
  {
    iota (parent.begin (), parent.end (), 0);
  }

  int
  find (int x)
  {
    if (parent[x] != x)
      parent[x] = find (parent[x]);
    return parent[x];
  }

  void
  unite (int x, int y)
  {
    int px = find (x), py = find (y);
    if (px != py)
      parent[px] = py;
  }

  int
  count_components ()
  {
    unordered_set<int> components;
    for (int i = 0; i < parent.size (); ++i)
    {
      components.insert (find (i));
    }
    return components.size ();
  }
};

// Segment tree for cover counting (1D range cover)
class SegmentTreeCover
{
public:
  int n;
  vector<int> tree;

  SegmentTreeCover (int size) : n (size), tree (size * 4) {}

  void
  add (int l, int r, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
      node_r = n - 1;
    if (r <= node_l || node_r < l)
      return;
    if (l <= node_l && node_r < r)
    {
      tree[node]++;
      return;
    }
    int mid = (node_l + node_r) / 2;
    add (l, r, node * 2, node_l, mid);
    add (l, r, node * 2 + 1, mid + 1, node_r);
  }

  void
  remove (int l, int r, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
      node_r = n - 1;
    if (r <= node_l || node_r < l)
      return;
    if (l <= node_l && node_r < r)
    {
      tree[node]--;
      return;
    }
    int mid = (node_l + node_r) / 2;
    remove (l, r, node * 2, node_l, mid);
    remove (l, r, node * 2 + 1, mid + 1, node_r);
  }

  bool
  query (int i, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
      node_r = n - 1;
    if (i < node_l || i > node_r)
      return false;
    if (node_l == node_r)
      return tree[node] > 0;
    int mid = (node_l + node_r) / 2;
    return (tree[node] > 0) || query (i, node * 2, node_l, mid)
           || query (i, node * 2 + 1, mid + 1, node_r);
  }
};

template <typename ret_t>
void
min_color_rectangles (vector<tuple<int, int, int, int> > rects,
                      ret_t& num_color)
{
  sort (rects.begin (),
        rects.end (),
        [] (auto& a, auto& b)
        {
          return get<0> (a) < get<0> (b);
        });

  unordered_set<int> unique_ys_set;
  for (auto& r : rects)
  {
    unique_ys_set.insert (get<2> (r));
    unique_ys_set.insert (get<3> (r));
  }

  vector<int> ys (unique_ys_set.begin (), unique_ys_set.end ());
  sort (ys.begin (), ys.end ());

  unordered_map<int, int> y_to_idx;
  for (int i = 0; i < ys.size (); ++i)
    y_to_idx[ys[i]] = i;

  SegmentTreeCover stc ((int)ys.size ());
  UnionFind uf ((int)rects.size ());

  vector<bool> active (rects.size (), false);

  for (int i = 0; i < rects.size (); ++i)
  {
    auto [x1, x2, y1, y2] = rects[i];
    int l = y_to_idx[y1];
    int r = y_to_idx[y2] - 1; // exclusive

    // 找出在 [l, r) 內被覆蓋的區間，進行 union
    for (int j = l; j < r; ++j)
    {
      if (stc.query (j))
      {
        for (int k = 0; k < i; ++k)
        {
          auto [_, __, py1, py2] = rects[k];
          int pl = y_to_idx[py1];
          int pr = y_to_idx[py2] - 1;
          if (active[k] && max (pl, l) <= j && j < min (pr + 1, r))
          {
            uf.unite (i, k);
          }
        }
      }
    }

    stc.add (l, r);
    active[i] = true;
  }

  num_color = uf.count_components ();
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);

  int n;
  cin >> n;
  std::vector<std::tuple<int, int, int, int> > rect (n);
  for (int i = 0; i < n; ++i)
  {
    cin >> std::get<0> (rect[i]) >> std::get<1> (rect[i])
        >> std::get<2> (rect[i]) >> std::get<3> (rect[i]);
  }

  min_color_rectangles (rect, n);
  cout << n << "\n";
}

