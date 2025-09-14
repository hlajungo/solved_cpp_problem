#include <bits/stdc++.h>
using namespace std;

class St_help
{
protected:
  inline bool
  is_intersect (auto&& l1, auto&& r1, auto&& l2, auto&& r2)
  {
    return r2 >= l1 && r1 >= l2;
  }
  inline bool
  is_include (auto&& l1, auto&& r1, auto&& l2, auto&& r2)
  {
    return l1 >= l2 && r2 >= r1;
  }

  inline int
  left (int& node)
  {
    return node * 2;
  }
  inline int
  right (int node)
  {
    return node * 2 + 1;
  }
  inline bool
  is_leaf (int& l, int& r)
  {
    return l == r;
  }
};

/*
 * @brief 1-index segment tree
 */
class Sum_st : St_help
{
private:
  int n;
  std::vector<int> tree;

  void
  build (std::vector<int>& v, int node, int l, int r)
  {
    if (is_leaf (l, r))
    {
      tree[node] = v[l];
      return;
    }
    build (v, left (node), l, (l + r) / 2);
    build (v, right (node), (l + r) / 2 + 1, r);
  }

  /*
   * @brief 單點增加值。
   * add_node 的內部遞迴實現。
   */
  void
  add_node (const int& idx, const int& val, int node, int l, int r)
  {
    if (is_leaf (l, r))
    {
      tree[node] += val;
      return;
    }
    int mid = (l + r) / 2;
    if (idx <= mid)
      // Go to eft child
      add_node (idx, val, left (node), l, mid);
    else
      // Go to right child
      add_node (idx, val, right (node), mid + 1, r);

    tree[node] = tree[left (node)] + tree[right (node)];
  }
  /*
   * @param ql Query left
   * @param qr Query right
   * @param node Node idx
   * @param l Node left
   * @param r Node right
   */
  int
  query (int ql, int qr, int node, int l, int r)
  {
    if (!is_intersect (l, r, ql, qr))
      return 0;
    if (is_include (l, r, ql, qr))
      return tree[node];
    int mid = (l + r) / 2;
    // Partial include, let left child and right child answer the question
    return query (ql, qr, left (node), l, mid)
           + query (ql, qr, right (node), mid + 1, r);
  }

public:
  // empty construct
  template <typename int_t>
  Sum_st (int_t&& n) : n (n), tree (4 * n, 0) {}
  Sum_st (std::vector<int>& v)
  {
    build (v, 1, 0, v.size () - 1);
  }

  /*
   * @brief  更新一個 leaf `idx` = `val` 和 `idx` 的 dfs 線。
   */
  void
  add_node (const int& idx, const int& val)
  {
    add_node (idx, val, 1, 0, n - 1);
  }

  /*
   * @brief Query the max value in segment [l, r]
   */
  int
  query (int ql, int qr)
  {
    return query (ql, qr, 1, 0, n - 1);
  }
};
/*
有 "求swap次數", "刪除卡牌"

求swap次數 = "兩數之間的卡牌"數量
刪除卡牌 = 標記0/1需求(而非從容器移除)

標記 0/1 需求 = XXX_st
數組中的 1 數量 = Sum_st
*/

int
main ()
{
  ios::sync_with_stdio (false);
  cin.tie (nullptr);
  int T;
  cin >> T;
  while (T--)
  {
    int n;
    cin >> n;
    vector<int> cards (2 * n);
    for (int i = 0; i < 2 * n; i++)
      cin >> cards[i];

    Sum_st st (2 * n);
    // idx = [卡牌值, 卡牌idx]
    unordered_map<int, int> first_pos;
    long long swaps = 0;
    for (int i = 0; i < 2 * n; i++)
    {
      int x = cards[i];
      // unordered_map 的 find 是找 key, not value
      // 數字 x 沒找到，就紀錄他的位置。
      if (first_pos.find (x) == first_pos.end ())
      {
        first_pos[x] = i;
      }
      // 數字 x 已經存在一份了
      else
      {
        int l = first_pos[x];
        int r = i;
        int length = (r - l - 1);
        int removed_inside = (length > 0) ? st.query (l + 1, r - 1) : 0;
        swaps += (length - removed_inside);
        // l, r 移除
        st.add_node (l, 1);
        st.add_node (r, 1);
      }
    }
    cout << swaps + n << "\n";
  }
  return 0;
}
