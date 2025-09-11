#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define EL endl

template <typename int_t = int>
void
matrix_multiplication (const int_t A_row,
                       const int_t A_col,
                       const int_t B_col,
                       const map<pair<int_t, int_t>, int_t>& A,
                       const map<pair<int_t, int_t>, int_t>& B,
                       map<pair<int_t, int_t>, int_t>& C)
{
  for (auto C_row = 0; C_row < A_row; ++C_row)
  {
    for (auto C_col = 0; C_col < B_col; ++C_col)
    {
      int_t sum = 0;
      for (int_t i = 0; i < A_col; ++i)
      {
        auto A_it = A.find ({ C_row, i });
        if (A_it == A.end ())
        { // A not found, 0 is default
          continue;
        }

        auto B_it = B.find ({ i, C_col });
        if (B_it == B.end ())
        { // B not found
          continue;
        }

        sum += A_it->second * B_it->second;
      }
      if (sum != 0)
      {
        C[{ C_row, C_col }] = sum;
      }
    }
  }
}

template <typename int_t>
void
matrix_transpose (const map<pair<int_t, int_t>, int_t>& A,
                  map<pair<int_t, int_t>, int_t>& B)
{
  for (auto& p : A)
  {
    B[{ p.first.second, p.first.first }] = p.second;
  }
}

template <typename int_t>
void
print_matrix (const int_t m_len, const map<pair<int_t, int_t>, int_t>& m)
{
  if (m_len == 0)
    return;
  vector<int_t> m2 (m_len * m_len, 0);
  for (auto& p : m)
  { // hashmap to vector
    m2[p.first.first * m_len + p.first.second] = p.second;
  }

  for (auto out_i = 0; out_i < m_len; ++out_i)
  {
    for (auto out_j = 0; out_j < m_len; ++out_j)
    { // print vector
      cout << m2[out_i * m_len + out_j];
      if (out_j != m_len - 1)
        cout << ' ';
    }
    cout << endl;
  }
}

int
main ()
{
  using int_t = int64_t;

  int_t n;
  cin >> n;
  cin.ignore ();

  if (n == 0)
    return 0;

  map<pair<int_t, int_t>, int_t> A;
  map<pair<int_t, int_t>, int_t> A_T;
  map<pair<int_t, int_t>, int_t> C;
  /*
  for (int n_i = 0; n_i < n; n_i++)
  {
    string str;
    getline (cin, str);
    // (\d+) 是一個數字
    // clang-format off
    regex pattern (R"(\((\d+):(\d+)\)=(\d+).*)");
    // clang-format on
    smatch match;
    if (regex_match (str, match, pattern))
    {
      row = stoll (match[1]);
      col = stoll (match[2]);
      value = stoll (match[3]);
      A[{ row - 1, col - 1 }] = value;
    }
  }
*/
  string str;
  for (; getline (cin, str);)
  {
    int_t row, col, value;

    // "(row:col)=value"
    size_t l_paren = str.find ('(');
    size_t colon = str.find (':', l_paren);
    size_t r_paren = str.find (')', colon);
    size_t equal_sign = str.find ('=', r_paren);

    // 擷取 row, col, value
    string row_str = str.substr (l_paren + 1, colon - l_paren - 1);
    string col_str = str.substr (colon + 1, r_paren - colon - 1);
    string value_str = str.substr (equal_sign + 1); // 後面全部抓下來

    try
    {
      //cout << '[' << row_str << ']' << col_str << ']' << value_str << ']'
           //<< endl;
      row = stoll (row_str);
      col = stoll (col_str);
      value = stoll (value_str);

      A[{ row - 1, col - 1 }] = value;
    }
    catch (...)
    {
      continue; // 若轉換失敗，跳過這筆資料
    }
  }

  // print_matrix (n, A);
  // cout << endl;

  matrix_transpose<int_t> (A, A_T);
  // print_matrix (n, A_T);
  // cout << endl;

  matrix_multiplication<int_t> (n, n, n, A, A_T, C);
  print_matrix<int_t> (n, C);
}
