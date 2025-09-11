#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

template <typename t_v>
void
print_vector (std::vector<t_v> v)
{
  cout << v[0];
  for (size_t i=1; i<v.size(); ++i)
  {
    cout <<' ' <<  v[i];
  }
  cout << '\n';
}

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i)
  {
    int m;
    cin >> m;
    vector<int> nums;

    for (int j = 0; j < m; ++j)
    {
      int tmp;
      cin >> tmp;
      nums.emplace_back(tmp);
    }
    char c;
    cin >> c;
    if (c == 'D')
    {
      sort (nums.begin (),
            nums.end (),
            [] (int& a, int& b)
            {
              return a > b;
            });
    }
    else
    {
      sort (nums.begin (),
            nums.end (),
            [] (int& a, int& b)
            {
              return a < b;
            });
    }
    print_vector (nums);
  }
}
