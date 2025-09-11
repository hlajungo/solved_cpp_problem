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

/* Main()  function */
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  // [pos. cnt, outline]
  // [pos, cnt, o1] -> [pos+1. cnt, o4]
  // [pos, cnt, o2] -> [pos+1, cnt+1, o3] || [pos+1, cnt, o4]
  // bool 輪廓線數組 = 整數 = 輪廓線狀態 狀態壓縮成數字，"狀態壓縮"
  //
  int n, K;
  cin >> n >> K;
  int limit = 1 <<(n+1); // 第n為用於表示左上角，0~n-1 表示輪廓線上的每一列
  ll dp[n*n +1][K+1][limit] = {};
  dp[0][0][0] = 1;
  for (int pos=0; pos < n*n; ++pos) { // 位置狀態
    int j = pos%n;  // 第幾列
    for (int k=0; k<=K; k++) { //  國王個數狀態
      for (int st =0; st<limit; ++st) { // 輪廓狀態
        if (!dp[pos][k][st]) continue; // 非法狀態
        int ul = ( st >> n ) & 1; // up left，左上角是否放了國王
        int cols = st - (ul << n); // 輪廓線上是否放了國王
        if (!ul && !(cols & ((1 << j >> 1) | (1 << j) | (1 << j << 1))) && k < K)
          dp[pos+1]p[k+1][cols | (1 << j)] += dp[pos][k][st]; // 放國王
        int nul = (cols >> j) & 1; // 下一個左上角
        int ncols = cols & (~(1 << j)); // 下一個輪廓線
        if (j == n -1) // 如果是最後一列，下個狀態是第一列，左上角默認為零
          nul = 0;
        dp[pos + 1][k][ (nul << n) | ncols] += dp[pos][k][st]; // 不放國王
      }
    }
  }
  cout << accumulate(dp [n*n][K], dp[n*n, K] + limit, 0ll) << ENDL;
  return 0;
}
