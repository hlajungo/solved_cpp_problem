#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

#define DBG_cout cout

template <typename t_v>
void
print_vector (std::vector<t_v> v)
{
  for (auto& i : v)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

template <typename t_v>
void
print_vector_pairs (std::vector<t_v> v)
{
  for (auto& i : v)
  {
    std::cout << i.first << ':' << i.second << '\n';
  }
}

// 用來展開 tuple 的 helper function
template <typename Tuple, size_t... Is>
void
print_tuple_impl (const Tuple& t, std::index_sequence<Is...>)
{
  ((std::cout << (Is == 0 ? "" : " ") << std::get<Is> (t)), ...);
  std::cout << '\n';
}

// 萬能轉發接口：列印任意型別的 tuple
template <typename... Args>
void
print_tuple (const std::tuple<Args...>& t)
{
  print_tuple_impl (t, std::index_sequence_for<Args...>{});
}

// 印出 vector<tuple<...>> 的函式
template <typename... Args>
void
print_vector_tuples (const std::vector<std::tuple<Args...> >& vec)
{
  for (const auto& t : vec)
  {
    print_tuple (t);
  }
}

/*
 * @brief Union_find, with "path compression" and "merge by rank"
 */
class Union_find
{
private:
  std::vector<int> parent; /// The parent of `idx` node
                           /// If the parent of `idx` is `idx`, `idx` is root

  std::vector<int> rank; /// The height of root (if `idx` is root)

public:
  /*
   * @brief Create `n` set, each set has 1 element with value `i` of `n`
   * @param[in] n
   */
  Union_find (int n)
  {
    parent.resize (n);
    rank.resize (n, 0);
    for (int i = 0; i < n; ++i)
    {
      parent[i] = i;
    }
  }

  /*
   * @brief Merge two set
   * @param[in] x The element in set
   * @param[in] y The element in set
   */
  void
  unite (const int& x, const int& y)
  {
    int root_x = get_root (x);
    int root_y = get_root (y);

    if (root_x == root_y)
    { // `x`, `y` in same set
      return;
    }

    // Decide the root of set
    if (rank[root_x] < rank[root_y])
    { // the bigger rank `idx` is new root
      parent[root_x] = root_y;
    }
    else if (rank[root_x] > rank[root_y])
    {
      parent[root_y] = root_x;
    }
    else
    { // equal rank, `x` is chosen as root
      parent[root_y] = root_x;
      rank[root_x]++;
    }
  }

  /*
   * @brief Find the root in the set that contains 'x'
   * @param[in] x
   */
  int
  get_root (int x)
  {
    if (parent[x] != x)
    { // Not root
      // Compression path, let all parent of x point to root
      parent[x] = get_root (parent[x]);
    }
    return parent[x];
  }

  /*
   * @brief Is the set of `x` and the set of `y` is connected?
   * @param[in] x
   * @param[in] y
   */
  bool
  is_connected (const int& x, const int& y)
  {
    return get_root (x) == get_root (y);
  }

  int
  get_components_num ()
  {
    unordered_set<int> components;
    for (size_t i = 0; i < parent.size (); ++i)
    {
      components.insert (get_root (i));
    }
    return components.size ();
  }
};

class Segment_tree_cover
{
private:
  int n;
  vector<int> tree; // 覆蓋計數

public:
  Segment_tree_cover (int size) : n (size), tree (size * 4) {}

  /*
   * @brief Add 1 for interval [l,r)
   * @param l Left interval to add
   * @param r Right interval to add
   * @param node Root node
   * @param node_l Left interval of node
   * @param node_r Right interval of node
   *               -1 means last node
   */
  void
  add (int l, int r, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
    {
      node_r = n - 1;
    }

    if (r <= node_l || node_r < l)
    { // no overlap
      return;
    }
    if (l <= node_l && node_r < r)
    { // total overlap
      tree[node]++;
      return;
    }
    // partial overlap
    int mid = (node_l + node_r) / 2;
    add (l, r, node * 2, node_l, mid);
    add (l, r, node * 2 + 1, mid + 1, node_r);
  }

  /*
   * @brief Remove 1 for interval [l,r)
   * @param l Left interval to add
   * @param r Right interval to add
   * @param node Root node
   * @param node_l Left interval of node
   * @param node_r Right interval of node
   *               -1 means last node
   */
  void
  remove (int l, int r, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
    {
      node_r = n - 1;
    }
    if (r <= node_l || node_r < l)
    { // no overlap
      return;
    }
    if (l <= node_l && node_r < r)
    { // total overlap
      tree[node]--;
      return;
    }
    // partial overlap
    int mid = (node_l + node_r) / 2;
    remove (l, r, node * 2, node_l, mid);
    remove (l, r, node * 2 + 1, mid + 1, node_r);
  }

  /*
   * @brief Query i is overlap with interval [node_l, node_r)
   * @param l Left interval to add
   * @param node Root node
   * @param node_l Left interval of node
   * @param node_r Right interval of node
   *               -1 means last node
   */
  bool
  query (int i, int node = 1, int node_l = 0, int node_r = -1)
  {
    if (node_r == -1)
    {
      node_r = n - 1;
    }
    if (i < node_l || i > node_r)
    { // no overlap
      return false;
    }
    if (node_l == node_r)
    { // total overlap
      return tree[node] > 0;
    }
    // partial overlap
    int mid = (node_l + node_r) / 2;
    return (tree[node] > 0) || query (i, node * 2, node_l, mid)
           || query (i, node * 2 + 1, mid + 1, node_r);
  }
};

// 初始化 Seqment_tree_cover
// 初始化 Union_find ，各家分離
// 遍歷 [y1,y2]
//  把不滿足的 [y1.y2] 踢出 Stc
//  查詢 [y1,y2] 疊加狀態，從 (0,1) 一直到 (n-2,n-1) 得到每個區間的疊加狀態
//  ，會是像 0111010110，多個 1 只需要 union_find.unite 一次
//  把 [y1,y2] 加入 Stc
// 統計 Union_find component 數量，為答案

/*
 * @brief Adjacent rectangles are colored the same color, get the minimum of
 * color to fill rectangles.
 * @param[in] Rectangles Conatin left down (x1, y1) and right up (x2, y2)
 * @param[out] num_color Minimum color
 */
template <typename ret_t>
void
min_color_rectangles (std::vector<std::tuple<int, int, int, int> > rects,
                      ret_t& num_color)
{
  if (rects.size() == 1)
  {
    num_color = 1;
    return;
  }

  // sort by x1
  sort (rects.begin (),
        rects.end (),
        [] (const std::tuple<int, int, int, int>& left,
            const std::tuple<int, int, int, int>& right)
        {
          auto& [lx1, ly1, lx2, ly2] = left;
          auto& [rx1, ry1, rx2, ry2] = right;
          if (lx1 == rx1)
          {
            return lx2 < rx2;
          }
          return lx1 < rx1;
        });

  // create unique ordered y set
  vector<int> ys;
  for (auto i : rects)
  {
    auto [_0, y1, _1, y2] = i;
    // add y1 and y2, y1 is enter event, y2 is leave event
    // DBG
    // cout << "Add " << y1 << ' ' << y2 << '\n';
    ys.emplace_back (y1);
    ys.emplace_back (y2);
  }

  sort (ys.begin (), ys.end ());
  ys.erase (unique (ys.begin (), ys.end ()), ys.end ());

  // DBG
  // print_v(ys);

  // unique ordered y to idx
  unordered_map<int, int> y_to_idx;
  for (size_t i = 0; i < ys.size (); ++i)
  {
    y_to_idx[ys[i]] = i;
  }

  // DBG
  for (auto& i : y_to_idx)
  {
    cout << i.second << ':' << i.first << ' ';
  }
  cout << '\n';

  vector<int> active;
  // 幾個 rect，幾個點
  Union_find uf ((int)rects.size ());

  for (size_t i = 0; i < rects.size (); ++i)
  {
    auto [x1, y1, x2, y2] = rects[i];
    // DBG
    cout << "Hello rect" << i << '\n';

    // remove unactive rect
    for (auto& j : active)
    {
      auto [px1, py1, px2, py2] = rects[j];
      if (px2 < x1)
      {
        // DBG
        cout << "Remove rect" << j << " from active" << '\n';
        active.erase (remove (active.begin (), active.end (), j),
                      active.end ());
      }
    }

    int l = y_to_idx[y1];
    int r = y_to_idx[y2];

    // DBG
    cout << "Interval for rect = (" << l << "," << r << ")\n";

    // DBG
    for (auto& j : active)
    {
      auto [px1, py1, px2, py2] = rects[j];
      auto pl = y_to_idx[py1];
      auto pr = y_to_idx[py2];
      if (r < pl || l > pr)
      {
        cout << "Rect" << i << ',' << j << " no overlap\n";
      }
      else
      {
        uf.unite(i, j);
        cout << "Rect" << i << ',' << j << " overlap\n";
      }
    }
    active.push_back (i);

    // DBG
    cout << "active rect = ";
    print_vector (active);
    cout << '\n';
  }
   num_color = uf.get_components_num ();
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
