#include <iostream>
#include <map>
#include <regex>
#include <string>

using namespace std;

using sparse_matrix_t = map<pair<int, int>, int>;

int main() {
    int n;
    cin >> n;
    cin.ignore(); // 清除換行符

    sparse_matrix_t A;

    string line;
    regex pattern(R"(\((\d+):(\d+)\)=(\d+))");

    while (getline(cin, line)) {
        if (line.empty()) break;

        smatch match;
        if (regex_match(line, match, pattern)) {
            int row = stoi(match[1]);
            int col = stoi(match[2]);
            int val = stoi(match[3]);
            A[{row - 1, col - 1}] = val; // 轉為 0-based index
        }
    }

    // 準備結果矩陣 C
    int C[10][10] = {}; // 因為 n < 10

    // A x AT
    for (int i = 0; i < n; ++i) {       // 第 i 行
        for (int j = 0; j < n; ++j) {   // 第 j 行 (AT 的第 j 行 == A 的第 j 列)
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                int a1 = A.count({i, k}) ? A[{i, k}] : 0;
                int a2 = A.count({j, k}) ? A[{j, k}] : 0;
                sum += a1 * a2;
            }
            C[i][j] = sum;
        }
    }

    // 輸出結果
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << C[i][j];
            if (j != n - 1) cout << " ";
        }
        cout << '\n';
    }

    return 0;
}

