#include "Engine.h"
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#include "Global.h"
#include "QueryParser.h"

void Engine::processSearch(const string &query) {
  vector<vector<QueryParser::QueryClause>> groups;
  QueryParser::parseQueryString(query, groups);

  auto mergeOccurrences = [](vector<QueryResult> &A, vector<pair<int, vector<int>>> &B) {
    vector<QueryResult> C;
    for (int i = 0, j = 0; i < (int)A.size() && j < (int)B.size(); ++i, ++j) {
      if (A[i].fileID < B[j].first) {
        ++i;
      } else if (A[i].fileID > B[j].first) {
        ++j;
      } else {
        C.push_back({A[i].fileID, A[i].score, {}});
        C.back().score += B[j].second.size();
        auto &pos = C.back().pos;
        merge(A[i].pos.begin(), A[i].pos.end(), B[j].second.begin(), B[j].second.end(), pos.begin());
        pos.resize(unique(pos.begin(), pos.end()) - pos.begin());
        ++i, ++j;
      }
    }
    A.swap(C);
  };

  auto filterOccurrencesByFileID = [](vector<QueryResult> &A, vector<int> fileIDs, bool remove) {
    std::unordered_set<int> fileIDSet(fileIDs.begin(), fileIDs.end());
    vector<QueryResult> C;
    for (auto &res : A) {
      if (fileIDSet.count(res.fileID) != remove) {
        C.push_back(res);
      }
    }
    A.swap(C);
  };

  // process each group
  for (auto &group : groups) {
    vector<QueryResult> res;
    for (int i = 0; i < Global::numFiles; ++i) {
      res.push_back({i, 0, {}});
    }

    for (auto &clause : group) {
      switch (clause.type) {
        case QueryParser::QueryType::INCLUDE: {
          auto tmp = processInclude(clause.keyword);
          mergeOccurrences(res, tmp);
          break;
        }

        case QueryParser::QueryType::EXCLUDE: {
          auto tmp = processExclude(clause.keyword);
          // filterOccurrencesByFileID(res, tmp, true);  // TODO
          break;
        }

        case QueryParser::QueryType::IN_TITLE: {
          auto tmp = processInTitle(clause.keyword);
          //mergeOccurrences(res, tmp);  // TODO
          break;
        }

        case QueryParser::QueryType::FILETYPE: {
          auto tmp = processFileType(clause.keyword);
          // filterOccurrencesByFileID(res, tmp, false);  // TODO
          break;
        }

        case QueryParser::QueryType::EXACT_MATCH: {
          auto tmp = processExactMatch(clause.keyword);
          //mergeOccurrences(res, tmp);  // TODO
          break;
        }

        case QueryParser::QueryType::NUMBER_RANGE: {
          auto tmp = processNumberRange(clause.keyword);
          //mergeOccurrences(res, tmp);  // TODO
          break;
        }

        case QueryParser::QueryType::SYNONYM: {
          auto tmp = processSynonym(clause.keyword);
          //mergeOccurrences(res, tmp);  // TODO
          break;
        }
      }
    }
  }

  // take union
  // TODO

  // show result
  // TODO

  waitForEnter();
}

vector<pair<int, vector<int>>> Engine::processInclude(const string &keyword) {
  cdebug << "[Engine::processInclude] " << keyword << '\n';
  TrieNode *node = Global::trieContent.findWord(keyword);
  if (node == nullptr) {
    return {};
  }

  vector<pair<int, vector<int>>> res;
  vector<pair<int, int>> occurrences = node->occurrences;
  sort(occurrences.begin(), occurrences.end());

  int lastFileID = occurrences[0].first;
  res.push_back({lastFileID, {}});
  for (auto [fileID, pos] : occurrences) {
    if (fileID != lastFileID) {
      lastFileID = fileID;
      res.push_back({lastFileID, {}});
    }
    res.back().second.emplace_back(pos);
  }

  if (Global::verbose) {
    for (auto &[fileID, posArr] : res) {
      std::cout << "File " << fileID << ":";
      for (int pos : posArr) std::cout << ' ' << pos;
      std::cout << '\n';
    }
  }

  return res;
}

vector<int> Engine::processExclude(const string &keyword) {
  cdebug << "[Engine::processExclude] " << keyword << '\n';
  return {};
}

vector<pair<int, vector<int>>> Engine::processInTitle(const string &keyword) {
  cdebug << "[Engine::processInTitle] " << keyword << '\n';
  return {};
}

vector<int> Engine::processFileType(const string &keyword) {
  cdebug << "[Engine::processFileType] " << keyword << '\n';
  return {};
}

vector<pair<int, vector<int>>> Engine::processExactMatch(const string &keyword) {
  // also WILDCARD
  cdebug << "[Engine::processExactMatch] " << keyword << '\n';
  return {};
}

vector<pair<int, vector<int>>> Engine::processNumberRange(const string &keyword) {
  cdebug << "[Engine::processNumberRange] " << keyword << '\n';
  return {};
}

vector<pair<int, vector<int>>> Engine::processSynonym(const string &keyword) {
  cdebug << "[Engine::processSynonym] " << keyword << '\n';
  return {};
}
