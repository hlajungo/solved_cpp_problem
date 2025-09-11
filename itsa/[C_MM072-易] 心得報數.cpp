#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

/*
 * Josephus Problem
 * J_{n,k} = (( J_{n-1, k} + k - 1 ) mod n) + 1
 * n: total person
 * k: kth person removed
 * ref: https://www.scaler.in/josephus-problem/
 * recommand video: https://www.youtube.com/watch?v=uCsD3ZGzMgE
 * */

size_t josephus (size_t n, size_t k)
{
  if (n == 1)
    return 1;
  else
    return ( josephus (n-1, k) + k -1 ) % n + 1;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  cin >> n;
  while (n--)
  {
    int m, k;
    cin >> m >> k;
    cout << josephus (m, k) << '\n';
  }
}
