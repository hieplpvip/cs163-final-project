#include "Engine.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include "Global.h"
#include "QueryParser.h"

void Engine::processSearch(const string &query) {
  vector<vector<QueryParser::QueryClause>> groups;
  QueryParser::parseQueryString(query, groups);

  // process each group
  for (auto &group : groups) {
    for (auto &clause : group) {
      switch (clause.type) {
        case QueryParser::QueryType::INCLUDE:
          processInclude(clause.keyword);
          break;

        case QueryParser::QueryType::EXCLUDE:
          processExclude(clause.keyword);
          break;

        case QueryParser::QueryType::IN_TITLE:
          processInTitle(clause.keyword);
          break;

        case QueryParser::QueryType::FILETYPE:
          processFileType(clause.keyword);
          break;

        case QueryParser::QueryType::EXACT_MATCH:
          processExactMatch(clause.keyword);
          break;

        case QueryParser::QueryType::NUMBER_RANGE:
          processNumberRange(clause.keyword);
          break;

        case QueryParser::QueryType::SYNONYM:
          processSynonym(clause.keyword);
          break;
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
