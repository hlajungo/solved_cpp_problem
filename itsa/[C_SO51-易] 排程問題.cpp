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
        [&] (const int& a, const int& b)
        {
          return a < b;
        });

  int sum =0;
  int sum2=0;
  for (size_t i=0; i<ints.size() - 1; ++i)
  {
    //cout << sum << "+" << ints[i]<<'\n';
    sum += ints[i];
    sum2+=sum;
  }

  cout << sum2 << '\n';

}
