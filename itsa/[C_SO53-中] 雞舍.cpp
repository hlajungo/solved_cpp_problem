#include <bits/stdc++.h>
#include <algorithm>
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



template <typename array_1d_t>
void
selection_sort (array_1d_t& nums, int& change_num)
{
  for (size_t i = 0; i < nums.size (); ++i)
  {
    auto it = std::max_element (nums.begin () + i, nums.end ());
    if (*it != nums[i])
    {
      std::iter_swap (nums.begin() + i, it);
      change_num += 1;
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
  while (n--)
  {
    int m;
    int ret = 0;
    cin >> m;
    vector<int> nums (m);
    for (int i = 0; i < m; ++i)
    {
      cin >> nums[i];
    }
     selection_sort (nums, ret);

    //bubble_sort (nums.begin (),
                 //nums.end (),
                 //[&ret] (const int& a, const int& b)
                 //{
                   //if (a < b)
                   //{
                     //++ret;
                   //}
                   //return a < b;
                 //});

    cout << ret << '\n';
    print_vector(nums);
  }
}

