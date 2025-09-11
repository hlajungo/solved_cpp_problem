#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

#ifdef DEBUG
#define dout cout
#else
struct Dummy
{
  template <typename T>
  Dummy&
  operator<< (const T&)
  {
    return *this;
  }
};
Dummy dout;
#endif

template <typename t_v>
void
print_vector (std::vector<t_v> v)
{
  for (auto& i : v)
  {
    dout << i << ' ';
  }
  dout << '\n';
}

template <typename t_v>
void
print_vector_pairs (std::vector<t_v> v)
{
  for (auto& i : v)
  {
    dout << i.first << ':' << i.second << '\n';
  }
}

// 用來展開 tuple 的 helper function
template <typename Tuple, size_t... Is>
void
print_tuple_impl (const Tuple& t, std::index_sequence<Is...>)
{
  ((dout << (Is == 0 ? "" : " ") << std::get<Is> (t)), ...);
  dout << '\n';
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
  if (rects.size () == 0)
  {
    num_color = 0;
    return;
  }
  else if (rects.size () == 1)
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
    // dout << "Add " << y1 << ' ' << y2 << '\n';
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
    dout << i.second << ':' << i.first << ' ';
  }
  dout << '\n';

  vector<int> active;
  // 幾個 rect，幾個點
  Union_find uf ((int)rects.size ());

  for (size_t i = 0; i < rects.size (); ++i)
  {
    auto [x1, y1, x2, y2] = rects[i];
    // DBG
    dout << "Hello rect" << i << '\n';

    // remove unactive rect
    active.erase (remove_if (active.begin (),
                             active.end (),
                             [&] (int j)
                             {
                               auto [px1, py1, px2, py2] = rects[j];
                               return px2 < x1;
                             }),
                  active.end ());

    int l = y_to_idx[y1];
    int r = y_to_idx[y2];

    dout << "Interval for rect = (" << l << "," << r << ")\n";

    for (auto& j : active)
    {
      auto [px1, py1, px2, py2] = rects[j];
      auto pl = y_to_idx[py1];
      auto pr = y_to_idx[py2];

      dout << "(" << l << "," << r << ") (" << pl << "," << pr << ")\n";
      // bool is_overlap = (x2 <= px1 && px2 <= x1 && y2  py1 && py2 >= y1);
      if (r >= pl && l <= pr)
      {
        uf.unite (i, j);
        dout << "Rect" << i << ',' << j << " overlap\n";
      }
      else
      {
        dout << "Rect" << i << ',' << j << " no overlap\n";
      }
    }
    active.push_back (i);

    dout << "active rect = ";
    print_vector (active);
    dout << '\n';
  }
  num_color = uf.get_components_num ();
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);

  int n;
  while (cin >> n)
  {
    std::vector<std::tuple<int, int, int, int> > rect (n);
    for (int i = 0; i < n; ++i)
    {
      cin >> std::get<0> (rect[i]) >> std::get<1> (rect[i])
          >> std::get<2> (rect[i]) >> std::get<3> (rect[i]);
    }

    int ret = 0;
    min_color_rectangles<int> (rect, ret);
    cout << ret << "\n";
  }
}
