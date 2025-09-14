#include "/media/hlajungo/D/linux/repo_my/cpp_lib/dbg.hpp"
#include <bits/stdc++.h>
using namespace std;

/*
 * [2d幾何] [物件移動] [範圍檢測]
 *
 * [2d幾何] + [物件移動] -> [x軸掃描線] + [事件化] -> [開始/地雷/結束事件化] +
 * [事件排序] -> [事件遍歷]
 * [範圍檢測] -> [事件遍歷] + [y軸離散唯一化] + [BIT] -> 解決
 *
 */

class Sum_BIT
{
private:
  int n;                  // tree size
  vector<long long> tree; // 1-based array tree

public:
  Sum_BIT (int n) : n (n), tree (n + 1, 0) {}

  /*
   * @brief
   * @complexity O(n log n)
   */
  Sum_BIT (const vector<int>& arr)
  {
    n = arr.size ();
    tree.assign (n + 1, 0);
    for (int i = 0; i < n; i++)
    {
      node_add (i, arr[i]);
    }
  }

  /*
   * @brief node `idx` += val
   * @compelxity O(log n)
   */
  void
  node_add (int i, long long val)
  {
    i++; // 0-based -> 1-based
    while (i <= n)
    {
      tree[i] += val;
      i += (i & -i); // Get next(會包含此區間，但範圍更大，來自binary性質)
    }
  }

  /*
   * @brief node `l` to `r` += `val`
   */
  void
  range_node_add (int l, int r, int val)
  {
    DOUT << "RANGE ADD " << l << " " << r << "\n";
    node_add (l, val);
    DOUT_array_1d (tree);
    if (r + 1 <= n)
      node_add (r + 1, -val);
    DOUT_array_1d (tree);
  }

  /*
   * @brief Query prefix sum
   * @complexity O(log n)
   */
  long long
  query (int i) const
  {
    ++i; // from 0-based to 1-based
    long long res = 0;
    while (i > 0)
    {
      res += tree[i];
      i -= (i & -i); // Get father
    }
    return res;
  }

  /*
   * @brief Query range prefix sum
   * @complexity O(log n)
   */
  long long
  range_query (int l, int r) const
  {
    return query (r) - query (l - 1);
  }
};

// ---------------- 事件定義：掃描線按 x 排序 ----------------
enum Event_t : unsigned char
{
  START = 0, // 矩形左邊界（加入）
  MINE = 1,  // 地雷（查詢）
  END = 2    // 矩形右邊界（移除）
};

struct Event
{
  long long x;   // 掃描線座標
  Event_t event; // 事件型別
  long long y1;  // 下界 for START/END, 位置 for MINE
  long long y2;  // 同 y1

  // std::sort from small to big
  // using x to sort, or sort by START,MINE,END
  bool
  operator< (const Event& other) const
  {
    if (x != other.x)
      return x < other.x;
    return event < other.event;
  }
};

struct Move
{
  int dir;        // 方向: 0=East, 1=North, 2=West, 3=South
  long long dist; // 距離
};

void
build_event_1d (vector<Event>& event_1d,
                const vector<Move>& move_1d,
                long long d)
{
  long long x = 0, y = 0; // 起點 (0,0)
  for (const auto& move : move_1d)
  {
    long long nx = x, ny = y;
    if (move.dir == 0)
      nx += move.dist; // East
    else if (move.dir == 1)
      ny += move.dist; // North
    else if (move.dir == 2)
      nx -= move.dist; // West
    else if (move.dir == 3)
      ny -= move.dist; // South

    // 矩形原始邊界
    long long xl = min (x, nx), xr = max (x, nx);
    long long yl = min (y, ny), yr = max (y, ny);

    long long rx1, rx2, ry1, ry2;
    if (y == ny)
    {
      // x 移動
      rx1 = xl - d;
      rx2 = xr + d;
      ry1 = y - d;
      ry2 = y + d;
    }
    else
    {
      // y 移動
      rx1 = x - d;
      rx2 = x + d;
      ry1 = yl - d;
      ry2 = yr + d;
    }

    event_1d.push_back (Event{ rx1, START, ry1, ry2 });
    event_1d.push_back (Event{ rx2, END, ry1, ry2 });

    // 從新位置繼續移動
    x = nx;
    y = ny;
  }
}

int
main ()
{
  ios::sync_with_stdio (false);
  cin.tie (nullptr);

  while (true)
  {
    // cin d
    long long d;
    if (!(cin >> d))
      return 0;
    if (d == 0)
      break;

    // cin move
    int n;
    cin >> n;
    vector<Move> move_1d (n);
    for (int i = 0; i < n; i++)
    {
      cin >> move_1d[i].dir >> move_1d[i].dist;
    }

    // move to event
    vector<Event> event_1d;
    event_1d.reserve (2 * n);
    build_event_1d (event_1d, move_1d, d);

    // cin mine
    int m;
    cin >> m;
    event_1d.reserve (event_1d.size () + m);

    // allY 儲存所有出現過的 y
    vector<long long> allY;
    allY.reserve (2 * n + m);

    for (const auto& e : event_1d)
    {
      allY.push_back (e.y1);
      allY.push_back (e.y2);
    }


    // 地雷也做為事件（MINE）
    for (int i = 0; i < m; ++i)
    {
      long long xi, yi;
      cin >> xi >> yi;
      event_1d.push_back (Event{ xi, MINE, yi, yi });
      allY.push_back (yi);
    }

    // 按 x 排序事件；同 x：START → MINE → END
    sort (event_1d.begin (), event_1d.end ());

    // y 座標壓縮（因為 y 可達 ±2e9）
    sort (allY.begin (), allY.end ());
    // unique 會讓 allY
    // 變成一個去重前綴數組+剩餘未去重資料，並返回未去重的頭部迭代器
    // 使用 erase 把，未去重的部份刪掉。
    allY.erase (unique (allY.begin (), allY.end ()), allY.end ());

    // y 座標轉成 Fenwick Tree idx
    // lowerbound 把 yval 換成迭代器，通過迭代器相減得到索引
    auto y2idx = [&] (long long yval) -> int
    {
      return lower_bound (allY.begin (), allY.end (), yval) - allY.begin ();
    };

    DOUT << "ally size = " << allY.size () << "\n";

    Sum_BIT bit ((int)allY.size ());

    // 掃描線
    long long detonated = 0;
    // 用 x 軸遍歷掃描線
    for (const auto& e : event_1d)
    {
      if (e.event == START)
      {
        int L = y2idx (min (e.y1, e.y2));
        int R = y2idx (max (e.y1, e.y2));
        DOUT << "START " << e.x << " " << e.y1 << " " << e.y2 << " " << "\n";
        bit.range_node_add (L, R, 1);
      }
      else if (e.event == END)
      {
        int L = y2idx (min (e.y1, e.y2));
        int R = y2idx (max (e.y1, e.y2));
        DOUT << "END " << e.x << " " << e.y1 << " " << e.y2 << " " << "\n";
        bit.range_node_add (L, R, -1);
      }
      else
      { // MINE
        int P = y2idx (e.y1);
        if (bit.query (P) > 0)
        {
          ++detonated;
        }
      }
    }

    cout << detonated << '\n';
  }
  return 0;
}
