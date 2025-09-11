// C++ program to find 
// fibonacci number using recursion.
#include <bits/stdc++.h>
using namespace std;

// Function to find nth fibonacci number
int fib(int n) {
  if (n <= 1) {
    return n;
  }

  return fib(n - 1) + fib(n - 2);
}

int main() {
  int n = 24;
  cout << fib(n);
  return 0;
}

