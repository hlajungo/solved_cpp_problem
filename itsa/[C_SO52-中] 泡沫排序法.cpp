#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

int
main ()
{
  ios_base::sync_with_stdio (false);
  cin.tie (NULL);

  int n;
  cin >> n;
  vector<int> ints (n);

  for (int i = 0; i < n; ++i)
  {
    cin >> ints[i];
  }
  sort (ints.begin (),
        ints.end (),
        [] (const int& a, const int& b)
        {
          return a > b;
        });

  for (int i = 0; i < n; ++i)
  {
    cout << i+1 << ' ' << ints[i] << '\n';
  }
}
