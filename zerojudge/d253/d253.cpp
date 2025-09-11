#include <bits/stdc++.h>
using namespace std;

/* clang-format off */

/* TYPES  */
#define ll long long
#define pii pair<int, int>
#define pll pair<long long, long long>
#define vi vector<int>
#define vll vector<long long>
#define mii map<int, int>
#define si set<int>
#define sc set<char>

/* FUNCTIONS */
#define f(i,s,e) for(long long int i=s;i<e;i++)
#define cf(i,s,e) for(long long int i=s;i<=e;i++)
#define rf(i,e,s) for(long long int i=e-1;i>=s;i--)
#define pb push_back
#define eb emplace_back

/* PRINTS */
template <class T>
void print_v(vector<T> &v) { cout << "{"; for (auto x : v) cout << x << ","; cout << "\b}"; }

/* UTILS */
#define MOD 1000000007
#define PI 3.1415926535897932384626433832795
#define read(type) readInt<type>()
#define ENDL endl
#define TRI cout << "TRI" << ENDL;
ll min(ll a,int b) { if (a<b) return a; return b; }
ll min(int a,ll b) { if (a<b) return a; return b; }
ll max(ll a,int b) { if (a>b) return a; return b; }
ll max(int a,ll b) { if (a>b) return a; return b; }
ll gcd(ll a,ll b) { if (b==0) return a; return __gcd(b, a%b); }
ll lcm(ll a,ll b) { return a/gcd(a,b)*b; }
string to_upper(string a) { for (int i=0;i<(int)a.size();++i) if (a[i]>='a' && a[i]<='z') a[i]-='a'-'A'; return a; }
string to_lower(string a) { for (int i=0;i<(int)a.size();++i) if (a[i]>='A' && a[i]<='Z') a[i]+='a'-'A'; return a; }
bool prime(ll a) { if (a==1) return 0; for (int i=2;i<=round(sqrt(a));++i) if (a%i==0) return 0; return 1; }
void yes() { cout<<"YES\n"; }
void no() { cout<<"NO\n"; }

/*  All Required define Pre-Processors and typedef Constants */
typedef long int int32;
typedef unsigned long int uint32;
typedef long long int int64;
typedef unsigned long long int  uint64;

/* clang-format on */

/*
 * 1. For n, there are n subproblems.
 **/

int coins[] = {1, 5, 10, 25, 50};
const int maxN = 7500;
ll dp[maxN];
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

/*
dp[N]: N 元的組合方式
dp[N] = dp[N-1] + dp[N-5] + dp[N-10] + dp[N-25] + dp[N-50]
但實做上不是 dp[N] = dp[N-1] + dp[N-5] + dp[N-10] + dp[N-25] + dp[N-50]
而是 dp[N] += dp[N-1]
索引都是 N-1 N-5 這種往下的，第二層迴圈使用正向遍歷
有 5 個 dp 項，所以第一個 for 進行 5 次, 每次處理一個 dp 遞迴項
第零個遞迴項要開始算，假如是 dp[N-1]，需要的索引是
1+=0, 2+=1, ..., 7499+=7498
第一是 5+=0, 6+=1, 7499+=7494
第二是 10+=0. ...
...
第四是 50+=0, ..., 7499+=7449
你可以觀察到初始值在變化，都停在7499，其餘則相同。
這樣第二個迴圈就設計好了。
*/
  int n;
  dp[0] = 1;
  // 0 1 2 3 4
  // 0 -> 1 2 3 ... 推進 1 元
  // 1 -> 5 6 7 ... 推進 5 元
  // 2 -> 10 11 ...
  // 3 -> 25 26 ...
  // 4 -> 50 51 ...
  for (int i=0; i<5; i++) // Start from 5 type of coins
  {
    for (int j=coins[i]; j<maxN; j++) // start from coins value
    {
      dp[j] += dp[j-coins[i]];
    }
  }

  while (cin >> n)
  {
    cout << dp[n] << ENDL;
  }
  return 0;
}


