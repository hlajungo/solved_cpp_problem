#include <bits/stdc++.h>
using namespace std;

// 這個平台坑爹的不能用 using array_1d_t = std::vector<T, std::allocator<T> >;

/*
 * @brief check a set of number can be enumerate by a set of number
 * The size of dp is recommand be sum(set)
 * @param[in] arr
 * @param[out] dp
 */
template <typename T>
void
// enumerate_reachable_sums (const vector<T>& arr, set<T>& dp)
enumerate_reachable_sums (const T& arr_len, const T* arr, set<T>& dp)
{
  dp.insert (0);
  // for (auto& i : arr)
  for (int64_t i = 0; i < arr_len; ++i)
  {
    vector<T> prev(dp.begin(), dp.end());
    for (auto& j : prev)
    {
      // 註：以下是使用迴圈，但迴圈 TLE，改用 set
      // 如果順，會出現 0+1=1, 1 目前可行，所以 1+1 =2
      // 如果逆，1 目前不可行，然後計算 0，才出現 0+1=1
      // 逆迴圈可以解決依賴性問題
      // cout << i << ' ' << j << ' ' << endl;
      if (arr[i] + j <= 200000)
        dp.insert (arr[i] + j);
    }
  }
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);
  int n;
  cin >> n;
  for (int64_t i = 0; i < n; ++i)
  {
    int64_t weight_num, test_num;
    cin >> weight_num >> test_num;
    // vector<int64_t> weight (weight_num), test (test_num);
    // for (auto& w : weight)
    // cin >> w;
    // for (auto& x : test)
    // cin >> x;

    int64_t weight[205], test[55];
    for (int64_t j = 0; j < weight_num; j++)
    {
      cin >> weight[j];
    }
    for (int64_t j = 0; j < test_num; j++)
    {
      cin >> test[j];
    }

    set<int64_t> dp;
    enumerate_reachable_sums<int64_t> (weight_num, weight, dp);

    // dbg
    //for (auto j : dp)
    //{
        //cout << j << ' ';
    //}
    //cout << endl;

    int ans = 0;
    // for (auto& i : test)
    for (int64_t j = 0; j < test_num; ++j)
    {
      if (dp.find (test[j]) != dp.end ())
        ans++;
    }
    cout << ans << '\n';
  }
}
