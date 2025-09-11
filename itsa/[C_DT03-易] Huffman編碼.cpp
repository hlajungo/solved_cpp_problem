#include <bits/stdc++.h>

#include <queue>
using namespace std;
#define ll long long
#define EL endl

  template <typename T1>
void print_queue(std::queue<T1> q)
{
  while (!q.empty())
  {
    std::cout << q.front() << " ";
    q.pop();
  }
  std::cout << std::endl;
}

  template <typename T1, typename T2, typename T3>
void print_priority_queue (std::priority_queue <T1, T2, T3> pq)
{
  while (! pq.empty() )
  {
    cout << pq.top()->item << "\n";
    pq.pop();
  }
}

  template <typename T1, typename T2>
void print_unordered_map (const unordered_map <T1, T2>& um )
{
  for(auto it = um.cbegin(); it != um.cend(); ++it)
  {
    std::cout << (char)it->first << " " << it->second << "\n";
  }
}

  template <typename T1, typename T2>
void print_map (const map <T1, T2>& m )
{
  for(auto it = m.cbegin(); it != m.cend(); ++it)
  {
    std::cout << (char)it->first << " " << it->second << "\n";
  }
}

  template <typename T1, typename T2>
void print_pair_vector (const vector <pair <T1, T2>> vp)
{
  for (const auto& p : vp)
  {
    cout << "(" << (char)p.first << ", " << p.second << ")\n";
  }
}

  template <typename T1>
void print_binary_tree(const std::string& prefix, const T1* root, bool isLeft)
{
  if( root != nullptr )
  {
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    std::cout << root->item << std::endl;

    // enter the next tree level - left and right branch
    print_binary_tree ( prefix + (isLeft ? "│   " : "    "), root->left, true);
    print_binary_tree ( prefix + (isLeft ? "│   " : "    "), root->right, false);
  }
}

  template <typename T1>
void print_binary_tree (const T1* root)
{
  print_binary_tree ("", root, false);
}


class Hf_node
{
  public:
    Hf_node
      (size_t freq = 0
       , char item = '0'
       , Hf_node* left = nullptr
       , Hf_node* right = nullptr
       , size_t order = 0
      )
      : freq (freq), item (item),  left (left), right (right)
        , order(order)
  {}
    // note: 以下三個的排序要與初始化列表的排序相同 zzz
    size_t freq;
    char item;
    Hf_node* left, *right;
    size_t order;

    // 小的優先, 先加入的優先
    bool operator() (const Hf_node* a, const Hf_node* b)
    {
      if (a->freq == b->freq)
      {
        return a->order > b->order;
      }
      else
      {
        return a->freq > b->freq;
      }
    };
};


class Huffman_tree
{
  public:

    Hf_node*
      build_huffman_tree
      (
       std::priority_queue < Hf_node*
       , std::vector <Hf_node*>
       , Hf_node> min_heap
      )
      {

        std::priority_queue < Hf_node*
          , std::vector <Hf_node*>
          , Hf_node> min_heap2;

        int i = '0';
        int order = 1000000;
        // 最後就剩一個點
        while (! ( min_heap2.size() == 1 && min_heap.size() == 0 ) )
        {
          bool use_r_or_n = 0, use_r_or_n2 = 0;
          Hf_node* left_node = nullptr;
          if (min_heap.size() == 0
              || (min_heap2.size() > 0 && min_heap2.top() < min_heap.top())
             )
          {
            // from n
            left_node = min_heap2.top(); min_heap2.pop();
            use_r_or_n = 1;
          }
          else
          {
            // from r
            left_node = min_heap.top(); min_heap.pop();
            use_r_or_n = 0;
          }

          Hf_node* right_node = nullptr;
          if (min_heap.size() == 0
              || ( min_heap2.size() > 0 && min_heap2.top() < min_heap.top() )
             )
          {
            right_node = min_heap2.top(); min_heap2.pop();
            use_r_or_n2 = 1;
          }
          else
          {
            right_node = min_heap.top(); min_heap.pop();
            use_r_or_n2 = 0;
          }

          Hf_node* top_node = nullptr;
          if ( use_r_or_n == 1 && use_r_or_n2 == 1)
          {
            // 若兩個子節點皆取自N，則取代第一個子節點位置
            // 兩個子節點皆取自節點集合N時，在節點集合N中，位置最前面的當左子節點。
            top_node = new Hf_node (left_node->freq + right_node->freq, i++, right_node, left_node, order++);
          }
          else if ( use_r_or_n == 0 && use_r_or_n2 == 0)
          {
            // 不用更動順序
            top_node = new Hf_node (left_node->freq + right_node->freq, i++, left_node, right_node, order--);
          }
          else if ( use_r_or_n == 1 && use_r_or_n2 == 0)
          {
            // 兩個子節點一個取自字元集合R，一個取自節點集合N時，取自N的節點當作左子節點,取自R的字元當右子節點
            // 當有取節點集合N當作子節點時，則加入此父節點時，取代原本子節點位置
            // (? 這兩句話感覺矛盾了，假設 A + B = N, N + C + M, M 要取代 N，也就是 A + B = M 但 M 的右邊有 C，B 和 C 爭搶一點)
            top_node = new Hf_node (left_node->freq + right_node->freq, i++, left_node, right_node, order++);
            //top_node->left = left_node->left->left;
            //top_node->right = left_node->left->right;

          }
          else if ( use_r_or_n == 0 && use_r_or_n2 == 1)
          {
            // 兩個子節點一個取自字元集合R，一個取自節點集合N時，取自N的節點當作左子節點,取自R的字元當右子節點
            // 當有取節點集合N當作子節點時，則加入此父節點時，取代原本子節點位置
            top_node = new Hf_node (left_node->freq + right_node->freq, i++, right_node, left_node, order++);
          }
          //cout << (char)top_node->item << ' ' << top_node->left->item << '+' << top_node->right->item << ' ' << top_node->order << '\n';

          min_heap2.push (top_node);
        }
        return min_heap2.top();
      }

    void
      print_huffman
      (
       Hf_node* root
       , vector <size_t> arr
       , vector <pair<size_t, string>>* ret
      )
      {
        vector <size_t> arr2 = arr;
        // 題目要求，可移除
        if (root->left)
        {
          arr.emplace_back ('0');
          //cout << "GO left to " << root->left->item << '\n';
          print_huffman (root->left, arr, ret);
        }

        if (root->right)
        {
          // left add 1
          //cout << "GO right to " << root->right->item << '\n';
          arr2.emplace_back ('1');
          print_huffman (root->right, arr2, ret);
        }

        if (root->left == nullptr && root->right == nullptr)
        {
          // leaf
          //cout << "GET " << root->item << '\n';
          std::string str(arr.begin(), arr.end());
          ret->push_back ({root->item, str});
        }
      }

};


int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  string str;
  getline (cin, str);
  int k=1;
  while (k--)
  {

    // <freq, item>
    std::priority_queue < Hf_node*
      , std::vector <Hf_node*>
      , Hf_node> min_heap;

    vector <pair <char, size_t>> freq;
    for (auto c : str)
    {
      // 尋找 pair.first 是 c
      auto it = find_if (freq.begin(), freq.end()
          , [c](const pair<char, size_t>& p)
          {
          return p.first == c;
          });
      if (it != freq.end())
      {
        (it->second)++;
      }
      else
      {
        freq.push_back ({c, 1});
      }
    }

    int order=100000;
    for (const auto& [item, freq] : freq)
    {
      //cout << "Add " << item << freq << '\n';
      Hf_node* tmp = new Hf_node (freq, item, nullptr, nullptr, order++);
      // 添加到 pq 的東西不會按照順序，需要多個 order 來進行新後排序
      min_heap.push(tmp);
    }
    // 錯誤情況
    if (min_heap.size() == 1)
    {
      return 1;
    }

    //print_priority_queue (min_heap);

    Huffman_tree ht;
    // calc huffman tree and return root
    auto ht_root = ht.build_huffman_tree (min_heap);

    //print_binary_tree (ht_root);


    vector <size_t> arr;
    vector <pair <size_t, string>> ret;
    // get huffman code at `ret`
    ht.print_huffman (ht_root, arr, &ret);

    sort (ret.begin(), ret.end(), [](pair<size_t, string>& A, pair<size_t, string>& B)
        {
        // 先使用 binary 大小，然後string大小
        bitset<128> b1 (A.second.c_str());
        bitset<128> b2 (B.second.c_str());
        if ( b1.to_ullong() == b2.to_ullong() )
        {
        return A.second.size() > B.second.size();
        }
        // little to big
        return b1.to_ullong() < b2.to_ullong();
        });
    for (auto& pair : ret)
    {
      cout << pair.second << " -> " << (char)pair.first  << '\n';
    }
  }
}


