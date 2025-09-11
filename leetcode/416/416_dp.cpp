class Solution {
public:
    bool canPartition(vector<int>& nums) {
        // 若加起來為奇數，則不可能平分成兩堆
        // 若 half_sum 存在，則必存在另外一組加起來 = half_sum
        // 若有一 5 元，且 0 元和 1 元可到達，則 5 元和 6 元都可到達。
        // 就這樣進行，直到

        unsigned long long total_sum = accumulate(nums.begin(), nums.end(), 0),
                           half_sum = total_sum / 2;
        vector<bool> dp(half_sum+1, false); // dp[i]: 是否能組成 i 元
        if (total_sum & 1)                // total_sum = odd
            return false;
        dp[0] = true;
        for (auto num : nums) { // num
            for (int i = half_sum; i >= num ; --i) { //
                if (dp[i - num])
                    dp[i] = true;
            }
        }
        return dp[half_sum];
    }
};

// 在同一行內，如果後方的變量要使用到前方的變量，前方的變量會先算好並賦值，然後後方才繼續嘛？
