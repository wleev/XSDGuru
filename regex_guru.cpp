#define FE_DLOG_LEVEL FE_LOG_TRACE
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/regex_guru.hpp>
#include <tools/ssl/xsd/model/element_declaration.hpp>

#include <boost/cast.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <iterator>

using namespace std;
using namespace boost; 
using namespace re2;

RegexGuru::RegexGuru(string regexString__){
  xpressive::sregex seperatorRegex = xpressive::sregex::compile("\\([^\\(\\)\\s]+\\)");
  FE_TRACE(__F(("RegexGuru::RegexGuru - Creating regex %1%") % regexString__));
  regexObj = make_shared<RE2>(regexString__);
  xpressive::sregex_token_iterator elementIter(regexString__.begin(),
					       regexString__.end(),
					       seperatorRegex,
					       0);
  xpressive::sregex_token_iterator elementIterEnd;
  for(elementIter; elementIter != elementIterEnd; elementIter++){
    string elementNameTemp =  (*elementIter);
    elementNameTemp = elementNameTemp.substr(1, elementNameTemp.length() - 2);
    FE_TRACE(__F(("RegexGuru::RegexGuru - Adding elementname : %1%") % elementNameTemp ));
    elementNames.push_back(elementNameTemp);
  }

  sort(elementNames.begin(), elementNames.end(), [](string a, string b)->bool{
    size_t alength = a.length();
    size_t blength = b.length();
    return (alength > blength);
  });

  string elementRegexString = "^(";
  for(int i=0; i < elementNames.size(); i++){
    elementRegexString += "("+elementNames[i] + ")";
    if(i != elementNames.size() - 1) elementRegexString += "|";
  }
  elementRegexString += ")";
  
  elementRegex = make_shared<RE2>(elementRegexString);
}


vector< vector<string> > RegexGuru::getValidChildrenPositions(string matchString__){
  FE_TRACE("RegexGuru::getValidChildrenPositions - Clearing valid positions");
  clearPositions();

  StringPiece inputString(matchString__);
  StringPiece  matchedValue;

  int elementCounter = 0;
  FE_TRACE("RegexGuru::getValidChildrenPositions - Checking valid permutations before first element");
  validPositions.push_back(vector<string>());
  checkPermutations(matchString__, 0, elementCounter);  
  FE_TRACE("RegexGuru::getValidChildrenPositions - Starting regex search...");
  while(RE2::Consume(&inputString, *elementRegex, &matchedValue)){
	elementCounter++;
    validPositions.push_back(vector<string>());
    FE_TRACE(__F(("RegexGuru::getValidChildrenPositions - Checking valid permutations before element %1%") % elementCounter ));
	checkPermutations(matchString__, inputString.data() - matchString__.data(), elementCounter);
  }
  FE_TRACE("RegexGuru::getValidChildrenPositions - Finished regex search...");

  return validPositions;
}

void RegexGuru::checkPermutations(string matchString__, int insertPos__, int elementPos__){
  string tempString = matchString__;
  FE_TRACE(__F(("RegexGuru::checkPermutations - Insert position : %1%") % insertPos__));
  for(string el : elementNames){
    if(insertPos__ >= tempString.size()){ tempString.append(el); }
    else { tempString.insert(insertPos__, el); }
    FE_TRACE(__F(("RegexGuru::checkPermutations - Checking match for %1%") % tempString ));
	//FE_INFO(__F(("RegexGuru::checkPermutations - calling regexMatch with regex %1% and string %2%") % regexObj.str() % tempString));
    if(RE2::FullMatch(tempString, *regexObj)){
      FE_TRACE(__F(("RegexGuru::checkPermutations - %1% matches, adding to list...") % el ));
      validPositions[elementPos__].push_back(el);
    }
    tempString = matchString__;
  }
}

void RegexGuru::clearPositions(){
    validPositions.clear();
}

string RegexGuru::constructRegex(GroupDeclaration* group__)
{
  string regexString, elementRegexString;
  bool canAdd = true;
  DeclarationSet groupChildren = group__->getChildren();
  for(int i=0; i < groupChildren.size(); i++){
    ElementDeclaration* childElement_ = groupChildren[i];
    FE_TRACE(__F(("IndicatorMatcher::matchGroupForElement - Getting regex for %1%") % childElement_->getName()));
    if(childElement_->isComplex()){
      ComplexElementDeclaration* complexTemp_ = polymorphic_downcast<ComplexElementDeclaration*>(childElement_);
      if(complexTemp_->getContentType() == ContentType::GROUP){
	GroupDeclaration* gd_ = polymorphic_downcast<GroupDeclaration*>(complexTemp_);
	if(gd_->isAnonymous()){
	  canAdd = false;
	  string anonRegex = constructRegex(gd_);
	  regexString += getRegex(anonRegex, gd_->getIndicatorMinOccurs(), gd_->getIndicatorMaxOccurs());
	  
	  if(i != (groupChildren.size() -1)){ elementRegexString += "|"; }
	}
      }
    }
    if(canAdd){
      int minOccurs_ = childElement_->getMinOccurs();
      //due to regex peculiarity we have to set minOccurs > 0 if we're using alternations (CHOICE)
      if(group__->getIndicator() == Indicator::CHOICE && minOccurs_ == 0){
		  minOccurs_ = 1;
      }
      regexString += getRegex(childElement_->getName(),
			      minOccurs_,
			      childElement_->getMaxOccurs()
			      );
      elementRegexString += childElement_->getName();
      if(i != (groupChildren.size() -1)){ elementRegexString += "|"; }
      if(group__->getIndicator() == Indicator::CHOICE && i != (groupChildren.size() -1)){
	regexString += "|";
      }
    }
  }
  regexString = getRegex(regexString, group__->getIndicatorMinOccurs(), group__->getIndicatorMaxOccurs());
  return regexString;
  //return RegexCombo(regexString, elementRegexString);
}

string RegexGuru::getRegex(string value__, int minOccurs__, int maxOccurs__)
{
  string minString = static_cast<ostringstream*>(&(ostringstream() << minOccurs__))->str();
   
  if(maxOccurs__ == INT_MAX){
    if(minOccurs__ == 0){ return "("+value__+")*"; }
    else if(minOccurs__ == 1){ return "("+value__+")+"; }
    else if(minOccurs__ > 1){ return "("+value__+"){"+minString+",}"; }
  } else if(maxOccurs__ > 0){
    string maxString = static_cast<ostringstream*>(&(ostringstream() << maxOccurs__))->str();
    return "("+value__+"){"+minString+","+maxString+"}";
  } else {
    return "";
  }
}
