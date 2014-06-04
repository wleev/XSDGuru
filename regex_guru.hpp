#ifndef REGEX_GURU_HPP_
#define REGEX_GURU_HPP_

#include <tools/ssl/xsd/model/group_declaration.hpp>

#include <re2/re2.h>

#include <string>
#include <memory>
#include <vector>
#include <map>

class RegexGuru
{
public:
  RegexGuru(std::string regex);
  ~RegexGuru(){}

  static std::string constructRegex(GroupDeclaration* group__);

  std::vector<std::vector<std::string> > getValidChildrenPositions(std::string matchString__);
private:
  void checkPermutations(std::string matchString__, int insertPos__, int elementPos__);
  void clearPositions();
  static std::string getRegex(std::string value__, int minOccurs__, int maxOccurs__);

  std::shared_ptr<re2::RE2> regexObj;
  std::shared_ptr<re2::RE2> elementRegex;

  std::vector<std::string> elementNames;
  std::vector<std::vector<std::string> > validPositions;
};

#endif
