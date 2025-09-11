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

template <typename int_t>
void
bacterial_reproduction (const int_t& start_num, const int_t& end_day, int_t& sum)
{
  // i: 天數
  // j: 生命是第 i 天的細菌數量
  int_t dp[200][5] = {};
  dp[0][0] = start_num;

  for (int_t i = 1; i < end_day; ++i)
  {
    // note: 繁殖發生在 day 2, day 3, not day 4, day 4 is dead day

    dp[i][0] = dp[i - 1][2] + dp[i - 1][3];
    dp[i][1] = dp[i - 1][0];
    dp[i][2] = dp[i - 1][1];
    dp[i][3] = dp[i - 1][2];
    dp[i][4] = dp[i - 1][3];
  }
  sum = dp[end_day - 1][0] + dp[end_day - 1][1] + dp[end_day - 1][2]
        + dp[end_day - 1][3] + dp[end_day - 1][4];
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);
  int n;
  cin >> n;
  for (int n_i = 0; n_i < n; n_i++)
  {
    // x: 初始細菌, y: 第 n 天細菌數
    int64_t x, y;
    cin >> x >> y;
    int64_t sum = 0;
    bacterial_reproduction<int64_t> (x, y, sum);
    cout << sum << endl;
  }
}
