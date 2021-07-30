#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include <string>
#include <vector>

using std::string, std::vector;

namespace QueryParser {
  enum class QueryType {
    INCLUDE,
    EXCLUDE,
    IN_TITLE,
    FILETYPE,
    EXACT_MATCH,  // also WILDCARD
    NUMBER_RANGE,
    SYNONYM,
  };

  struct QueryClause {
    string keyword;
    QueryType type;
  };

  void clearStopWords(string &query);
  void parseQueryString(const string &query, vector<vector<QueryClause>> &groups);
  bool isNumberRange(const string &keyword);
};

#endif
