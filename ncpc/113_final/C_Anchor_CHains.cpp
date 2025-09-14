#include "/media/hlajungo/D/linux/repo_my/cpp_lib/dbg.hpp"
#include <bits/stdc++.h>

using namespace std;

struct Anchor
{
  int a, b, c, d, w;
  int id;
  int cidx, didx;
  Anchor () : a (0), b (0), c (0), d (0), w (0), id (-1), cidx (0), didx (0) {}
  Anchor (int a, int b, int c, int d, int w, int id = -1)
      : a (a), b (b), c (c), d (d), w (w), id (id), cidx (0), didx (0)
  {
  }
};

/*
1-1 dp最優子結構和依賴性
這是一個 max dp，他們的通解是 "當前狀態 = 之前狀態中最好的 + 自己"
該怎麼接龍呢？弄一個鏈條，在尾部新增。
C[i] = M[i] 結尾的最大分數
（設立 dp 變量時，可以想為 "自己" + "在自己之前會算完的浮空變量"）
C[i] = w(M[i]) + max{C[j] | 1 <= j < i, M[j] precedes M[i]}
（拿著這個空虛的變量繼續下一步，構件初始的dp和dp的延伸階梯）

1-2 dp 的"在自己之前會算完的浮空變量"合法性。
一開始長度為 1，然後長度為 2，之前的都會先算完，沒問題。

2-1 max{C[j] | 1 <= j < i, M[j] 和 M[i] 之間的要求關係} 計算
(如果現在有 C[i]，該怎麼算 max{C[j]} 呢？至少需要符合那兩個條件)
2-2 b<a' 和 d<c' 兩要求關係的建立
2-3 b<a'
可以使用 [事件化]+[事件排序]，使得遍歷事件時，當前事件的先前事件必定 b<a'
當使用  best = seg.query (1, cpos - 1); 查詢 d<c' 時，best 的 b < a`。
2-4 d<c'
可以使用 [c,d離散化]+[St]，使得 best = seg.query (1, cpos - 1)，best 的 d<c'。
並且能同時完成 max{C[j]} 的尋找，一舉兩得。
（還是感覺很神奇，b<a' 和 d<c' 兩條件能委託給兩個不同的方法，然後遍歷事件和St查詢完全錯開，能完成抽象的拼接，dp 階梯能正確蓋出來）
（能用排序消掉的條件 → 排序/事件化）
（能用範圍查詢處理的條件 → 離散化+資料結構）
（這是針對兩條件的情況，更多條件需要更多抽象拼接）
（可以這樣想，我有哪些條件需要符合，那我進行這些抽象拼接，之間是否會衝突，之間是否能填滿條件語意）
 */


// 1-based sum st
struct SegmentTree
{
  int n;
  vector<long long> tree;
  SegmentTree (int n_ = 0)
  {
    init (n_);
  }
  void
  init (int n_)
  {
    n = max (1, n_);
    tree.assign (4 * n + 5, 0);
  }
  void
  update (int idx, long long val, int node, int l, int r)
  {
    if (l == r)
    {
      tree[node] = max (tree[node], val);
      return;
    }
    int mid = (l + r) >> 1;
    if (idx <= mid)
      update (idx, val, node << 1, l, mid);
    else
      update (idx, val, node << 1 | 1, mid + 1, r);
    tree[node] = max (tree[node << 1], tree[node << 1 | 1]);
  }
  void
  update (int idx, long long val)
  {
    if (idx >= 1)
      update (idx, val, 1, 1, n);
  }

  long long
  query (int ql, int qr, int node, int l, int r)
  {
    if (ql > qr || qr < l || r < ql)
      return 0;
    if (ql <= l && r <= qr)
      return tree[node];
    int mid = (l + r) >> 1;
    return max (query (ql, qr, node << 1, l, mid),
                query (ql, qr, node << 1 | 1, mid + 1, r));
  }
  long long
  query (int ql, int qr)
  {
    if (ql > qr)
      return 0;
    ql = max (ql, 1);
    qr = min (qr, n);
    return query (ql, qr, 1, 1, n);
  }
};

int
main ()
{
  ios::sync_with_stdio (false);
  cin.tie (nullptr);

  int T;
  if (!(cin >> T))
    return 0;
  while (T--)
  {
    int n;
    cin >> n;
    vector<Anchor> M;
    M.reserve (n);
    for (int i = 0; i < n; ++i)
    {
      int a, b, c, d, w;
      cin >> a >> b >> c >> d >> w;
      M.emplace_back (a, b, c, d, w, i);
    }

    // 離散化 c, d
    vector<int> coords;
    coords.reserve (2 * n);
    for (auto& m : M)
    {
      coords.push_back (m.c);
      coords.push_back (m.d);
    }
    sort (coords.begin (), coords.end ());
    coords.erase (unique (coords.begin (), coords.end ()), coords.end ());
    auto get_idx = [&] (int x)
    {
      return int (lower_bound (coords.begin (), coords.end (), x)
                  - coords.begin ())
             + 1;
    };

    DOUT << "coords\n";
    DOUT_array_1d (coords);

    // c,d 轉離散編號
    for (auto& m : M)
    {
      m.cidx = get_idx (m.c);
      m.didx = get_idx (m.d);
    }

    // a,b,id 排序
    sort (M.begin (),
          M.end (),
          [] (const Anchor& x, const Anchor& y)
          {
            if (x.a != y.a)
              return x.a < y.a;
            if (x.b != y.b)
              return x.b < y.b;
            return x.id < y.id;
          });

    DOUT << "M (a,b,c,d) \n";
    DOUT_array_1d (M,
                   [] (const Anchor& t)
                   {
                     DOUT << t.a << " " << t.b << " " << t.c << " " << t.d
                          << "\n";
                   });

    // 建事件列表： (coord, type, idx) ; type: 0=a, 1=b
    vector<tuple<int, int, int> > events;
    events.reserve (2 * n);
    for (int i = 0; i < n; ++i)
    {
      events.emplace_back (M[i].a, 0, i);
      events.emplace_back (M[i].b, 1, i);
    }
    // 排序 event, 先以coord(a,b值)排序，後 type
    sort (events.begin (),
          events.end (),
          [] (const auto& l, const auto& r)
          {
            if (get<0> (l) != get<0> (r))
              return get<0> (l) < get<0> (r);
            return get<1> (l) < get<1> (r);
          });

    DOUT << "Event (coord,type,idx) \n";
    DOUT_array_1d_tuple (events);

    SegmentTree seg ((int)coords.size ());
    vector<long long> C (n, 0);

    /*
     * 狀態定義
     * C[i] = w, w = M[i] 結尾的最大分數
     * 狀態轉移
     * C[i] = w(M[i]) + max{C[j] | 1 <= j < i, M[j] precedes M[i]}
     *
     * 使用 St 表達以 node 結尾的最佳分數
     * 轉移在尋找 max{C[j]}
     */
    DOUT << "EVENT WORK\n";
    for (auto& ev : events)
    {
      int coodr = get<0> (ev);
      int type = get<1> (ev);
      int idx = get<2> (ev);
      DOUT << "coodr" << coodr << " type " << type << "\n";
      // a event, update C[idx]
      if (type == 0)
      {
        // find best d that d < c
        int cpos = M[idx].cidx;
        long long best = seg.query (1, cpos - 1);
        // update C[idx]
        C[idx] = (long long)M[idx].w + best;
        DOUT << C[idx] << " = " << M[idx].w << " " << best << "\n";
      }
      else
      {
        //DOUT << C[idx] << "\n";
        // b event, update st
        int dpos = M[idx].didx;
        seg.update (dpos, C[idx]);
      }
    }

    long long ans = 0;
    for (int i = 0; i < n; ++i)
      ans = max (ans, C[i]);
    cout << ans << '\n';
  }
  return 0;
}
