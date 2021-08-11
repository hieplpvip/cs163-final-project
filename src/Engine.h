#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <vector>
#include "Trie.h"

using std::pair, std::string, std::vector;

namespace Engine {
  struct QueryResult {
    int fileID;
    int score;
    vector<int> pos;
  };

  void processQuery(const string &query, vector<QueryResult> &final_res);
  void displayQueryResult(const string &query, const vector<QueryResult> &final_res);
  void displayFileResult(const QueryResult &res);
  void extractNumRange(const string keyword, float& num1, float& num2, vector<string>&res);
  void findNumInRange(TrieNode* root, std::string number, float &num1, float &num2, vector<string>& res);

  vector<pair<int, vector<int>>> processInclude(const string &keyword);
  vector<int> processExclude(const string &keyword);
  vector<pair<int, vector<int>>> processInTitle(const string &keyword);
  vector<int> processFileType(const string &keyword);
  vector<pair<int, vector<int>>> processExactMatch(const string &keyword);
  vector<vector<pair<int, vector<int>>>> processNumberRange(const string &keyword);
  vector<vector<pair<int, vector<int>>>> processSynonym(const string &keyword);
};

#endif
