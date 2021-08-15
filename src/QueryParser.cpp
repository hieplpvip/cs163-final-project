#include "QueryParser.h"
#include <sstream>
#include "Global.h"

void QueryParser::parseQueryString(const string &query, vector<vector<QueryClause>> &groups) {
  groups.clear();

  // split query into groups of AND clauses
  vector<string> raw_groups;
  int last = 0;
  for (int i = 1; i + 2 < (int)query.size();) {
    if (query[i - 1] == ' ' && query[i] == 'O' && query[i + 1] == 'R' && query[i + 2] == ' ') {
      if (i - 1 > last) {
        raw_groups.push_back(query.substr(last, i - last - 1));
      }
      i += 3;
      last = i;
    } else {
      ++i;
    }
  }
  if (last < (int)query.size()) {
    raw_groups.push_back(query.substr(last));
  }

  auto find_next = [](const string &s, char c, int pos) -> int {
    while (pos < (int)s.length() && s[pos] != c) ++pos;
    return pos;
  };

  // now parse each group
  for (string &query : raw_groups) {
    vector<QueryClause> clauses;

    for (int i = 0; i < (int)query.size();) {
      // skip space
      if (query[i] == ' ') {
        ++i;
        continue;
      }

      if (query[i] == '+') {
        int pos = find_next(query, ' ', i);
        if (i + 1 < pos) {
          string keyword = query.substr(i + 1, pos - i - 1);
          clauses.push_back({keyword, QueryType::INCLUDE});
        }
        i = pos + 1;

      } else if (query[i] == '-') {
        int pos = find_next(query, ' ', i);
        if (i + 1 < pos) {
          string keyword = query.substr(i + 1, pos - i - 1);
          clauses.push_back({keyword, QueryType::EXCLUDE});
        }
        i = pos + 1;

      } else if (query[i] == '~') {
        int pos = find_next(query, ' ', i);
        if (i + 1 < pos) {
          string keyword = query.substr(i + 1, pos - i - 1);
          clauses.push_back({keyword, QueryType::SYNONYM});
        }
        i = pos + 1;

      } else if (i + 7 < (int)query.length() && query.substr(i, 8).compare("intitle:") == 0) {
        i += 8;
        int pos = find_next(query, ' ', i);
        if (i < pos) {
          string keyword = query.substr(i, pos - i);
          clauses.push_back({keyword, QueryType::IN_TITLE});
        }
        i = pos + 1;

      } else if (i + 8 < (int)query.length() && query.substr(i, 9).compare("filetype:") == 0) {
        i += 9;
        int pos = find_next(query, ' ', i);
        if (i < pos) {
          string keyword = query.substr(i, pos - i);
          clauses.push_back({keyword, QueryType::FILETYPE});
        }
        i = pos + 1;

      } else if (query[i] == '"') {
        int pos = find_next(query, '"', i + 1);
        if (pos == (int)query.length()) {
          // unclosed quote. treat it as INCLUDE
          pos = find_next(query, ' ', i);
          string keyword = query.substr(i, pos - i);
          clauses.push_back({keyword, QueryType::INCLUDE});
        } else if (i + 1 < pos) {
          // found corresponding quote
          string keyword = query.substr(i + 1, pos - i - 1);
          clauses.push_back({keyword, QueryType::EXACT_MATCH});
        }
        i = pos + 1;

      } else {
        int pos = find_next(query, ' ', i);
        string keyword = query.substr(i, pos - i);
        if (keyword == "AND") {
          i = pos + 1;
          continue;
        }

        if (isNumberRange(keyword)) {
          clauses.push_back({keyword, QueryType::NUMBER_RANGE});
        } else if (Global::trieStopWord.findWord(keyword) == nullptr) {
          clauses.push_back({keyword, QueryType::INCLUDE});
        }
        i = pos + 1;
      }
    }

    if (!clauses.empty()) {
      groups.push_back(clauses);
    }
  }
}

bool QueryParser::isNumberRange(const string &keyword) {
  // Format: "$X..$Y". X and Y must be non-negative integers.

  // check "$"
  if (keyword.empty() || keyword[0] != '$') return false;

  int i = 1, lenX = 0, lenY = 0;

  // check "X."
  while (i < (int)keyword.length() && keyword[i] != '.') {
    if (keyword[i] < '0' || keyword[i] > '9') return false;
    ++lenX;
    ++i;
  }

  // check ".$"
  if (i + 2 >= (int)keyword.length() || keyword[i + 1] != '.' || keyword[i + 2] != '$') return false;

  // check "Y"
  i += 3;
  while (i < (int)keyword.length()) {
    if (keyword[i] < '0' || keyword[i] > '9') return false;
    ++lenY;
    ++i;
  }

  return lenX > 0 && lenY > 0;
}
