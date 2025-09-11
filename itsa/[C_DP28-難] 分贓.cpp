#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

/*
 * 把數字分成兩組相差最小的集合
 * 問題等價於組成一組最靠近 sum/2 的集合
 * */

template <typename T1>
void
dp (vector<T1> in)
{
  /*
   * dp
   * dp 遞迴式: dp[i] = dp[i] || dp [i - X], X 是金額，如果 i 元能到，則 i+X
   * 元能到 變量: bool dp[i]: 金額 i 能否到達 dp[max] = 最大金額，初始值 =
   * false, dp[0] = true 解答: 離 total/2 最近的 (dp[i]=true)
   *
   * note: 只在乎能法到達，不要求具體組合，使用 1-d dp 即可
   * */
  vector<vector<size_t> > dp;
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);

  size_t n;
  cin >> n;
  for (size_t i = 0; i < n; ++i)
  {
    size_t m, total = 0;
    cin >> m;
    vector<size_t> val (m);
    for (size_t j = 0; j < m; ++j)
    {
      cin >> val[j];
      total += val[j];
    }

    // note: 輸入已保證遞增，若沒，需要先排序

    vector<bool> dp (10000 * m, false);

    dp[0] = true;

    for (size_t c : val)
    {
      for (size_t j = total / 2 + 1; j >= c; --j)
      {
        dp[j] = dp[j] || dp[j - c];
      }
    }

    int ans = 0;
    for (int i = total / 2; i >= 0; --i)
    {
      // 找離中間最近的一堆
      if (dp[i])
      {
        ans = i;
        break;
      }
    }

    // (total - ans) - ans = 一堆 - 另一堆
    cout << total - 2 * ans << '\n';
  }
}
