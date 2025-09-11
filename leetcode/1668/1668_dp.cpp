class Solution {
public:
    int maxRepeating(string sequence, string word) {
        int s_s = sequence.size(), w_s = word.size();
        vector<int> dp(sequence.size() + 1, 0);
        
        // dp[i]: 長度 i 的 sequence 出現的次數
        // dp[i] = dp[i-w_s] + 1;
        // 正向遍歷 dp

        // ex: ababcababc, ab, s_s=10, w_s=2
        // for (1 to 9)
        //  if (s.sub(0 to 8) == w)
        //    dp[1 to 9]= dp[0 to 8] + 1;


        for (int i = w_s - 1; i < s_s; ++i) {
            if (sequence.substr(i - w_s + 1, w_s) == word) {
                dp[i + 1] = dp[i - w_s + 1] + 1; 
            }
        }
        return *max_element(dp.begin(), dp.end());
    }
};
