/*
 * 我還沒理解這份 dp 不過這解決了 greedy 選擇最快，而不能給最長字串的問題
 * */
class Solution {
public:
    vector<string> getLongestSubsequence(vector<string>& words, vector<int>& groups) {
        // 

        vector<int> dp(words.size(), 1); // dp[i]: 以 words[i] 結尾的最長 alternating subsequence 長度
        vector<int> prev(words.size(), -1); // 用於回溯結果
        int maxLen = 1, lastIdx = 0;
        for (int i = 1; i < words.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (groups[j] != groups[i] && dp[i] < dp[j] + 1) {
                    dp[i] = dp[j] + 1;
                    prev[i] = j;
                }
            }
            if (dp[i] > maxLen) {
                maxLen = dp[i];
                lastIdx = i;
            }
        }
        // 回溯找出答案
        vector<string> result;
        while (lastIdx != -1) {
            result.push_back(words[lastIdx]);
            lastIdx = prev[lastIdx];
        }
        reverse(result.begin(), result.end());
        return result;
    }
};
