#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/indicator_matcher.hpp>
#include <boost/cast.hpp>
#include <boost/regex.hpp>
#include <climits>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace boost;

int IndicatorMatcher::countChar(const string& s, char c)
{
   int n=0;
   string::const_iterator i = find(s.begin(), s.end(), c);
   while(i != s.end())
   {
	  ++n;
	  i = find(i+1, s.end(), c);
   }
   return n;
}

int IndicatorMatcher::match(GroupDeclaration* elementType, vector<string> existingChildren)
{
   string concatString;
   for(string s : existingChildren){ concatString += s; }

   //we're going to count the sequence of choice occurrences with a greedy regex
   string regexString;

   RegexCombo regexResults = constructRegex(elementType);
   regexString = regexResults.groupRegex;

   FE_TRACE(__F(("IndicatorMatcher::match - Initializing regex with string : %1%") % regexString ));
   regex ip_regex(regexString, regex_constants::ECMAScript);
	
   FE_TRACE(__F(("IndicatorMatcher::match - Searching in string : %1%") % concatString ));
   string replaceString = "*";
   string resultString = regex_replace(concatString, ip_regex, replaceString, match_default | format_sed | match_not_null);

   FE_TRACE(__F(("IndicatorMatcher::match - Result string : %1%") % resultString ));
   int matches = countChar(resultString, '*');
   FE_TRACE(__F(("IndicatorMatcher::match - Found %1% matches.") % matches ));
   return matches;
}

vector<int> IndicatorMatcher::matchGroupForElement(GroupDeclaration* elementType__, int selectIndex__, vector<string> existingChildren__)
{   
   int selectIndexConcat;
   string concatString;
   for(int i = 0; i < existingChildren__.size(); i++){
	  if(i == selectIndex__){
		 selectIndexConcat = concatString.size();
	  }
	  concatString += existingChildren__[i];
   }

   //we're going to count the sequence of choice occurrences with a greedy regex
   string regexString, elementRegexString;
   RegexCombo regexResults = constructRegex(elementType__);
   regexString = regexResults.groupRegex;
   elementRegexString = regexResults.elementRegex;
	

   FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Initializing regex with string : %1%") % regexString ));
   FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Initializing element regex with string : %1%") % elementRegexString));
   regex ip_regex(regexString);
   regex elementRegex(elementRegexString);
   match_results<string::const_iterator> what;
   string::const_iterator start, end;
   start = concatString.begin();
   end = concatString.end();
   int indexSearch = 0;
   vector<int> result;

   FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Starting regex search in string %1%") % concatString));
   while(regex_search(start, end, what, ip_regex))
   {
	  string searchString = string(start, end);
	  FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Searching in string : %1%") % searchString ));
	  string found(what[0].first, what[0].second);
	  int numberElements = distance(sregex_iterator(found.begin(), found.end(), elementRegex), sregex_iterator());
	  FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Found %1% (%2% elements) at index %3%") % found %  numberElements % indexSearch ));
	  indexSearch += numberElements;
	  if(indexSearch > selectIndex__)
	  {
		 result.push_back(indexSearch - numberElements);
		 result.push_back(indexSearch);
		 break;
	  }

	  start = what[0].second;
   }
   FE_TRACE("IndicatorMatcher::matchGroupForElement - Finished regex search");
   return result;
}

RegexCombo IndicatorMatcher::constructRegex(GroupDeclaration* group__)
{
   string regexString, elementRegexString;
   bool canAdd = true;
   DeclarationSet groupChildren = group__->getChildren();
   for(int i=0; i < groupChildren.size(); i++)
   {
	  ElementDeclaration* childElement_ = groupChildren[i];
	  FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Getting regex for %1%") % childElement_->getName()));
	  if(childElement_->isComplex())
	  {
		 ComplexElementDeclaration* complexTemp_ = polymorphic_downcast<ComplexElementDeclaration*>(childElement_);
		 if(complexTemp_->getContentType() == ContentType::GROUP)
		 {
			GroupDeclaration* gd_ = polymorphic_downcast<GroupDeclaration*>(complexTemp_);
			if(gd_->isAnonymous())
			{
			   canAdd = false;
			   RegexCombo anonRegex = constructRegex(gd_);
			   regexString += getRegex(anonRegex.groupRegex, gd_->getIndicatorMinOccurs(), gd_->getIndicatorMaxOccurs());
			   elementRegexString += anonRegex.elementRegex;
			   if(i != (groupChildren.size() -1)){ elementRegexString += "|"; }
			}
		 }
	  }
	  if(canAdd){
		 int minOccurs_ = childElement_->getMinOccurs();
		 //due to regex peculiarity we have to set minOccurs > 0 if we're using alternations (CHOICE)
		 if(group__->getIndicator() == Indicator::CHOICE && minOccurs_ == 0)
		 {
			minOccurs_ = 1;
		 }
		 regexString += getRegex(childElement_->getName(),
								 minOccurs_,
								 childElement_->getMaxOccurs()
			);
		 elementRegexString += childElement_->getName();
		 if(i != (groupChildren.size() -1)){ elementRegexString += "|"; }
		 if(group__->getIndicator() == Indicator::CHOICE && i != (groupChildren.size() -1))
		 {
			regexString += "|";
		 }
	  }
   }
   return RegexCombo(regexString, elementRegexString);
}

string IndicatorMatcher::getRegex(string value__, int minOccurs__, int maxOccurs__)
{
   string minString = static_cast<ostringstream*>(&(ostringstream() << minOccurs__))->str();
	
   if(maxOccurs__ == INT_MAX)
   {
	  if(minOccurs__ == 0){ return "("+value__+")*"; }
	  else if(minOccurs__ == 1){ return "("+value__+")+"; }
	  else if(minOccurs__ > 1){ return "("+value__+"){"+minString+",}"; }
   } else if(maxOccurs__ > 0)
   {
	  string maxString = static_cast<ostringstream*>(&(ostringstream() << maxOccurs__))->str();
	  return "("+value__+"){"+minString+","+maxString+"}";
   } else
   {
	  return "";
   }
}
