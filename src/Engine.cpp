#include "Engine.h"
#include <vector>
#include "QueryParser.h"

using std::string, std::vector;

void Engine::processSearch(const std::string &query) {
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
}

void Engine::processInclude(const std::string &keyword) {
  throw("not implemented");
}

void Engine::processExclude(const std::string &keyword) {
  throw("not implemented");
}

void Engine::processInTitle(const std::string &keyword) {
  throw("not implemented");
}

void Engine::processFileType(const std::string &keyword) {
  throw("not implemented");
}

void Engine::processExactMatch(const std::string &keyword) {
  // also WILDCARD
  throw("not implemented");
}

void Engine::processNumberRange(const std::string &keyword) {
  throw("not implemented");
}

void Engine::processSynonym(const std::string &keyword) {
  throw("not implemented");
}
