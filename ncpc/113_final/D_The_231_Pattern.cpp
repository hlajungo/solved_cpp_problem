#include <bits/stdc++.h>
#include "/media/hlajungo/D/linux/repo_my/cpp_lib/dbg.hpp"

using namespace std;

// Note:
// pq 是極值容器，預設為最大，用 std::greater<Ctx> 作為第三個模板參數改為最小值
// 自訂比較器需要定義這個
// struct Cmp {bool operator() (PARAM) {FUNC}};, < 是最大, > 是最小，和 std::sort 相反
// 直接把 Cmp 作為第三個模板參數傳入

struct Candidate
{
  long long val; // the val of 2 in 231
  int i;         // the idx of 2 in 231
  int j;         // the idx of 3 in 231
};

struct Cmp
{
  bool
  operator() (const Candidate& a, const Candidate& b)
  {
    return a.val < b.val;
  }
};
/*
 * [尋找231 pattern] -> [生成23] + [對最大的 23 尋找 21 並保證 1 在 23 右邊]
 * -> [線性遍歷天然生成 "1 在 23 右邊"] + [檢查式套路 = "線性遍歷 + 前方檢查 + 後方運算"]
 * -> [23最大值用 pq 檢查 1] + [23 生成用 stk, 尋找任何 ab, a<b]
 */

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
    vector<long long> A (n);
    for (int i = 0; i < n; ++i)
      cin >> A[i];

    // 儲存 2 (2 val, 2 idx)
    stack<pair<long long, int> > stk;
    // 儲存 23, 最大的 2 在上, 以方便找最小的 1
    priority_queue<Candidate, vector<Candidate>, Cmp> pq;

    bool found = false;
    // iter input
    for (int k = 0; k < n; ++k)
    {
      // 使用 pq.top() (最大的 2 的 23) 去找比 2 小的 1 (A[k])
      if (!pq.empty () && pq.top ().val > A[k])
      {
        Candidate c = pq.top ();
        cout << "yes " << (c.i + 1) << " " << (c.j + 1) << " " << (k + 1)
             << "\n";
        found = true;
        break;
      }

      // 嘗試用 A[k] 和在 stk 裡待著的 2 組成 23
      while (!stk.empty () && A[k] > stk.top ().first)
      {
        auto p = stk.top ();
        DOUT << "Create 23 " << p.first << " " << A[k] << "\n";
        stk.pop ();
        pq.push (Candidate{ p.first, p.second, k });
      }

      // 放入當前值，以在未來作為 231 的 2
      stk.push ({ A[k], k });
    }

    if (!found)
      cout << "no\n";
  }
  // 用了 stk, pq
  // stk 的 push, stk 的 pop + pq 的 push, pq 檢測，三條邏輯線很精彩。

  return 0;
}
