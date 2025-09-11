// C++ program to find
// fibonacci number using space optimised.
#include <iostream>
using namespace std;

int fibo(int n) {
  
    int prevPrev, prev, curr;

    // Storing the independent values
    prevPrev = 0;
    prev = 1;
    curr = 1;

    // Using the bottom-up approach
    for (int i = 2; i <= n; i++) {
        curr = prev + prevPrev;
        prevPrev = prev;
        prev = curr;
    }

    return curr;
}

int main() {
    int n = 5;
    cout << fibo(n);
    return 0;
}
