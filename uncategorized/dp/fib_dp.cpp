#include <iostream>
#include <vector>
using namespace std;

int fibRec(int n, vector<int> &memo) {
  // Base case
  if (n <= 1) {
    return n;
  }

  // To check if output already exists
  if (memo[n] != -1) {
    return memo[n];
  }

  // Calculate and save output for future use
  memo[n] = fibRec(n - 1, memo) + fibRec(n - 2, memo);

  return memo[n];
}

int fib(int n) {
  vector<int> memo(n + 1, -1);
  return fibRec(n, memo);
}

int main() {
  int n = 24;
  cout << fib(n);
  return 0;
}
