#ifndef ENGINE_H
#define ENGINE_H

#include <string>

namespace Engine {
  void processSearch(const std::string &query);
  void processInclude(const std::string &keyword);
  void processExclude(const std::string &keyword);
  void processInTitle(const std::string &keyword);
  void processFileType(const std::string &keyword);
  void processExactMatch(const std::string &keyword);
  void processNumberRange(const std::string &keyword);
  void processSynonym(const std::string &keyword);
};

#endif
