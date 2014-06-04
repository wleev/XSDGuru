#ifndef INDICATOR_MATCHER_
#define INDICATOR_MATCHER_

#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <vector>
#include <string>


struct RegexCombo {
   std::string groupRegex, elementRegex;
   RegexCombo(std::string groupRegex__, std::string elementRegex__) : groupRegex(groupRegex__), elementRegex(elementRegex__){}
};

class IndicatorMatcher
{
public:
   IndicatorMatcher();

   static int match(GroupDeclaration* elementType,std::vector<std::string> existingChildren);
   static std::vector<int> matchGroupForElement(GroupDeclaration* parentElement__, int removeIndex__, std::vector<std::string> existingChildren__);
private:
   static RegexCombo constructRegex(GroupDeclaration* group__);
   static std::string getRegex(std::string value, int minOccurs, int maxOccurs);
   static int countChar(const std::string& s, char c);
};

#endif
