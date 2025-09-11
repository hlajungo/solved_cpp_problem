#include <bits/stdc++.h>
// #include <concepts>
using namespace std;
#define ll long long
#define EL endl

template <typename t_v>
void
print_v (std::vector<t_v> v)
{
  for (auto& i : v)
  {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

struct Time_interval
{
  int64_t start;
  int64_t end;
  int64_t value;
  string text;
};

/*
 * @brief Find the biggest number <= target
 * @param[in] target
 * @param[in] vec Vector to find, sorted required
 * @param[out] idx Index of target
 */
template <typename vec_t>
void
binary_search_leq (const int64_t& target, const vec_t& vec, int64_t& idx)
{
  int left = 0;
  int right = vec.size () - 1;
  idx = -1;
  while (left <= right)
  {
    // 如為奇數，mid 偏左
    int mid = (left + right) / 2;
    if (vec[mid].end <= target)
    {
      // 在右， vec[mid].end <= end，符合要找的 <= target
      idx = mid;
      left = mid + 1;
    }
    else
    {
      // 在左
      right = mid - 1;
    }
  }
}

/*
 * @brief Non overlap max yield
 * @param[in,out] vec Data about overlap, will be sorted by end
 * @param[out] ans Order to get best yield
 * @param[out] best_yield Value of best yield
 */
template <typename data_t = const std::vector<Time_interval>,
          typename ans_t = int64_t>
void
non_overlap_max (data_t& vec, std::vector<ans_t>& ans, ans_t& best_yield)
{
  // 清空
  ans = vector<ans_t> ();
  if (vec.size () == 0)
  {
    return;
  }

  sort (vec.begin (),
        vec.end (),
        [] (const Time_interval& a, Time_interval& b)
        {
          // 以 end time 排序
          return a.end < b.end;
        });

  vector<int64_t> dp (vec.size ());
  dp[0] = vec[0].value;
  vector<int64_t> path (vec.size (), -1);

  for (size_t i = 1; i < vec.size (); ++i)
  {
    // i: 遍歷 vec
    int64_t j;
    // 找到第一個 < vec.start 的 vec.end
    binary_search_leq<data_t> (vec[i].start, vec, j);
    int64_t cur = vec[i].value + (j >= 0 ? dp[j] : 0);
    if (cur > dp[i - 1])
    { // 使用這個區間比之前好
      dp[i] = cur;
      path[i] = j;
    }
    else
    { // 用區間反而變差，延續上次的
      dp[i] = dp[i - 1];
      path[i] = path[i - 1];
    }
  }

  // 此處已經找到最大產值 dp[vec.size()-1]，然後要回溯路徑
  best_yield = dp[vec.size () - 1];
  for (int64_t i = vec.size () - 1; i >= 0;)
  {
    int64_t j;
    binary_search_leq<data_t> (vec[i].start, vec, j);
    if (vec[i].value + (j >= 0 ? dp[j] : 0) >= (i > 0 ? dp[i - 1] : 0))
    { // i + (i 的上個區間) 比 i-1 好，此區間要加入
      ans.emplace_back (i);
      i = j;
    }
    else
    { // 上一段比較好，重新指向上一段
      --i;
    }
  }

  // print_v<int64_t> (dp);
  // print_v<int64_t> (path);
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);
  size_t n;
  cin >> n;
  vector<Time_interval> vec;
  for (size_t i = 0; i < n; ++i)
  {
    Time_interval ti;
    cin >> ti.text;
    cin >> ti.start;
    cin >> ti.end;
    cin >> ti.value;
    vec.emplace_back (ti);
  }

  int64_t best_yield;
  vector<int64_t> ans;
  non_overlap_max (vec, ans, best_yield);
  // print_v (ans);
  for (auto it = ans.rbegin (); it != ans.rend (); ++it)
  {
    cout << vec[*it].text << ' ';
  }
  cout << best_yield << '\n';
}
