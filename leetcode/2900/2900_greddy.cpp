class Solution {
  public:
    vector<string> getLongestSubsequence(vector<string>& words,
        vector<int>& groups) {
      vector<string> ret;
      if (words.size() < 1) // invaild
        return ret;
      ret.emplace_back(words[0]);
      if (words.size() == 1) // one 
        return ret;

      auto save = groups[0];

      for (auto i = groups[1]; i < groups.size(); ++i) {
        if (groups[i] != save) {
          ret.emplace_back(words[i]);
          save = groups[i];
        }
      }
      return ret;
    }
};
