#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/xsd_guru.hpp>
#include <tools/ssl/xsd/regex_guru.hpp>
#include <tools/ssl/xsd/indicator_matcher.hpp>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <tools/ssl/xsd/declaration_factory.hpp>

#include <boost/cast.hpp>

#include <libxml/xmlreader.h>
#include <libxml/xinclude.h>
#include <libxml/xmlstring.h>

#include <iostream>
#include <algorithm>
#include <set>
#include <stdlib.h>

using namespace std;
using namespace xmlpp;
using namespace boost;

XSDGuru::XSDGuru(string file__){
  FE_TRACE(__F(("XSDGuru::XSDGuru - Initializing XSDGuru with file: %1%") % file__));

  xsdParser.parse_file(file__);
}

XSDGuru::~XSDGuru(){
  FE_TRACE(__F(("XSDGuru::~XSDGuru: destructor")));
}


DeclarationSet XSDGuru::rootNodes(){
  DeclarationSet rootNodes_;
  Element* rootElement = xsdParser.get_document()->get_root_node();
  NodeSet rootNodeSet = rootElement->find("*[local-name() = 'element']");
  if(!rootNodeSet.empty()){
    for(Node* n : rootNodeSet){
      Element* element_ = polymorphic_downcast<Element*>(n);
      string elementName_ = element_->get_attribute_value("name");
      if(!declarationCache.count(elementName_))   {
	unique_ptr<ElementDeclaration> elementShared_( DeclarationFactory::getDeclaration(element_) );
	declarationCache.insert( std::pair<string, unique_ptr<ElementDeclaration> >( elementName_, std::move(elementShared_) ) );
      }
      rootNodes_.push_back( declarationCache[elementName_].get() );
    }
  }
   
  return rootNodes_;
}


ElementDeclaration* XSDGuru::getElementDeclaration(string elementName__){
  Element* rootElement = xsdParser.get_document()->get_root_node();
  NodeSet elementDefinitionSet = rootElement->find("//*[local-name() = 'element' and @name='"+elementName__+"']");
  if(!elementDefinitionSet.empty()){
    Element* element_ = polymorphic_downcast<Element*>(elementDefinitionSet.front());
    string elementName_ = element_->get_attribute_value("name");
    if(!declarationCache.count(elementName_)){
      unique_ptr<ElementDeclaration> elementShared_(DeclarationFactory::getDeclaration(element_));
      declarationCache.insert( std::pair<string, unique_ptr<ElementDeclaration> >( elementName_, std::move(elementShared_) ) );
    }
    return declarationCache[elementName_].get();
  } else {
    FE_ERROR(__F(("XSDGuru::getElementDeclaration - Element with name %1% could not be found.") % elementName__));
  }
  //return default constructor
  return 0;
}

bool XSDGuru::canHaveChildren(string elementName__){
  return getElementDeclaration(elementName__)->isComplex();
}


DeclarationSet XSDGuru::validChildrenForElement(string elementName__){
  DeclarationSet validChildren_;

  function<void(GroupDeclaration*)> fillValidChildren;
  fillValidChildren = [&](GroupDeclaration* gd){
    for(ElementDeclaration* el : gd->getChildren()){
      if(isDeclarationAnonymousGroup(el)){
	FE_TRACE(__F(("XSDGuru::validChildrenForElement - Child %1% is anonymous") % el->getName()));
	GroupDeclaration* groupTemp_ = polymorphic_downcast<GroupDeclaration*>(el);
	fillValidChildren(groupTemp_);
      } else {
	FE_TRACE(__F(("XSDGuru::validChildrenForElement - Adding child %1%") % el->getName()));
	validChildren_.push_back( el );
      }
    }
  };
  
  if(canHaveChildren(elementName__)){
    GroupDeclaration* groupElement_ = polymorphic_downcast<GroupDeclaration*>(getElementDeclaration(elementName__));
    fillValidChildren(groupElement_);
  }
  sort( validChildren_.begin(), validChildren_.end() );
  validChildren_.erase( unique( validChildren_.begin(), validChildren_.end() ), validChildren_.end() );
  return validChildren_;
}

DeclarationSet XSDGuru::validChildrenForElement(string elementName__, vector<string> existingChildren__){
  if(existingChildren__.empty()) { return validChildrenForElement(elementName__);}

  DeclarationSet validChildren_;

  FE_TRACE("XSDGuru::validChildrenForElement - Getting element declaration and building regex");
  GroupDeclaration* parentElement_ = polymorphic_downcast<GroupDeclaration*>(getElementDeclaration(elementName__));
  string regexTemp = RegexGuru::constructRegex(parentElement_);
  RegexGuru guru_(regexTemp);

  FE_TRACE("XSDGuru::validChildrenForElement - Getting valid children for existing configuration");
  string matchString_;
  for(string s : existingChildren__){ matchString_ += s; }
  vector< vector<string> > positions_ = guru_.getValidChildrenPositions(matchString_);

  set<string> validElementNames;
  for(auto& elementRow : positions_){
    for(string elName : elementRow){
      validElementNames.insert(elName);
    }
  }

  for(string elName : validElementNames){
    validChildren_.push_back( parentElement_->getChild(elName) );
  }

  return validChildren_;
}

vector<int> XSDGuru::validPositionsForElement(string parentName__, string elementName__, vector<string> existingChildren__){
  vector<int> validPositions;
  if(parentName__.empty() || elementName__.empty() || existingChildren__.empty()){ return validPositions;}

  FE_TRACE("XSDGuru::validPositionsForElement - Getting element declaration and building regex");
  GroupDeclaration* parentElement_ = polymorphic_downcast<GroupDeclaration*>(getElementDeclaration(parentName__));
  string regexTemp = RegexGuru::constructRegex(parentElement_);
  RegexGuru guru_(regexTemp);

  FE_TRACE("XSDGuru::validPositionsForElement - Getting valid children for existing configuration");
  string matchString_;
  for(string s : existingChildren__){ matchString_ += s; }
  vector< vector<string> > positions_ = guru_.getValidChildrenPositions(matchString_);
  
  for(int i=0; i< positions_.size(); i++){
    int elementCount_ = count(positions_[i].begin(),
			      positions_[i].end(),
			      elementName__);
    if(elementCount_ > 0){ validPositions.push_back(i); }
  }
   
  return validPositions;
}


DeclarationSet XSDGuru::validChildrenBeforeElement(string parentElement__, int elementIndex__, vector<string> existingChildren__)
{
  auto beforeFunction = [] (int x) -> int { return x-1; };
  DeclarationSet validChildren_ =  validChildrenNextElement(parentElement__, elementIndex__, existingChildren__, beforeFunction);
  sort( validChildren_.begin(), validChildren_.end() );
  validChildren_.erase( unique( validChildren_.begin(), validChildren_.end() ), validChildren_.end() );
  return validChildren_;
}

DeclarationSet XSDGuru::validChildrenAfterElement(string parentElement__, int elementIndex__, vector<string> existingChildren__)
{
  auto afterFunction = [] (int x) -> int { return x+1; };
  DeclarationSet validChildren_ =  validChildrenNextElement(parentElement__, elementIndex__, existingChildren__, afterFunction);
  sort( validChildren_.begin(), validChildren_.end() );
  validChildren_.erase( unique( validChildren_.begin(), validChildren_.end() ), validChildren_.end() );
  return validChildren_;
}

DeclarationSet XSDGuru::validChildrenNextElement(string parentName__, int elementIndex__, vector<string> existingChildren__, function<int(int)> selector__)
{
  DeclarationSet validChildren;
  if(parentName__.empty() ||
     existingChildren__.empty() ||
     elementIndex__ < 0 ||
     elementIndex__ >= existingChildren__.size())
    { return validChildren; } 

  FE_TRACE("XSDGuru::validChildrenNextElement - Getting element declaration and building regex");
  GroupDeclaration* parentElement_ = polymorphic_downcast<GroupDeclaration*>(getElementDeclaration(parentName__));
  string regexTemp = RegexGuru::constructRegex(parentElement_);
  RegexGuru guru_(regexTemp);

  FE_TRACE("XSDGuru::validChildrenNextElement - Getting valid children for existing configuration");
  string matchString_;
  for(string s : existingChildren__){ matchString_ += s; }
  vector< vector<string> > positions_ = guru_.getValidChildrenPositions(matchString_);
  int selectIndex = elementIndex__;
  if(selector__(0) == 1){ selectIndex++ ;}
  vector<string> validElements_ = positions_[selectIndex];

  for(string el : validElements_){
    validChildren.push_back(parentElement_->getChild(el));
  }
  
  return validChildren;
} 
 
AttributeDeclarationSet XSDGuru::attributesForElement(string elementName__)
{
  AttributeDeclarationSet result;
  ElementDeclaration* elementDecl_ = getElementDeclaration(elementName__);
  if(elementDecl_->isComplex()){
    ComplexElementDeclaration* complexDecl_ = polymorphic_downcast<ComplexElementDeclaration*>(elementDecl_);
    FE_TRACE(__F(("XSDGuru::attributeForElement - Getting attributes for element %1%") % elementName__));
    result = complexDecl_->getAttributes();
  }
  return result;
}


vector<int> XSDGuru::invalidIndexesAfterRemove(string parentName__, int removeIndex__, vector<string> existingChildren__ )
{
  vector<int> result;

  if(existingChildren__.empty() | removeIndex__ < 0 | removeIndex__ > existingChildren__.size() -1 )
    {
      return result;
    }
   
  GroupDeclaration* rootElement_ = polymorphic_downcast<GroupDeclaration*>(getElementDeclaration(parentName__));
  string selectedElementName_ = existingChildren__[removeIndex__];
  ElementDeclaration* selectedElement_ = rootElement_->getChild(selectedElementName_);

  if(selectedElement_->getMinOccurs() == 0)
    {
      result.push_back(removeIndex__);
      return result;
    }

  vector<int> invalidRange = IndicatorMatcher::matchGroupForElement(rootElement_, removeIndex__, existingChildren__);
  for(int i= invalidRange[0]; i< invalidRange[1]; i++)
    {
      result.push_back(i);
    }
   
  return result;
}

bool XSDGuru::isDeclarationAnonymousGroup(ElementDeclaration* element__)
{
  bool result = false;
  if(element__->isComplex())
    {
      ComplexElementDeclaration* complexTemp_ = polymorphic_downcast<ComplexElementDeclaration*>(element__);
      if(complexTemp_->getContentType() == ContentType::GROUP)
	{
	  GroupDeclaration* groupTemp_ = polymorphic_downcast<GroupDeclaration*>(complexTemp_);
	  if(groupTemp_->isAnonymous())
	    {
	      result = true;
	    }
	}
    }
  return result;
	
}

void XSDGuru::xmlSchemaValidityErrorFunc(void* ctx__, const char* msg__, ...)
{
  FE_TRACE(__F(("XSDGuru:xmlSchemaValidityErrorFunc called")));
  char buffer[5000];
  va_list argp__;
  va_start(argp__, msg__);
  char *val__ = va_arg(argp__, char*);
  XSDGuru* guru_ = static_cast<XSDGuru*>(ctx__);
  guru_->validationErrors.push_back(val__);
  FE_INFO(__F(("Error: %1%") % val__));
  va_end(argp__);
  // elided logging of errors
}

void XSDGuru::xmlSchemaValidityWarningFunc(void* ctx__, const char* msg__, ...)
{
  FE_TRACE(__F(("XSDGuru:xmlSchemaValidityWarningFunc called")));
  std::string temporary__;
  char buffer[5000];
  va_list argp__;
  va_start(argp__, msg__);
  char *val__ = va_arg(argp__, char*);
  XSDGuru* guru_ = static_cast<XSDGuru*>(ctx__);
  guru_->validationWarnings.push_back(val__);
  FE_WARN(__F(("Warning: %1%") % val__));
  va_end(argp__);
  // elided logging of warnings
}

void XSDGuru::genericErrorHandlerFunc(void* ctx__, const char* msg__, ...)
{
  FE_TRACE(__F(("XSDGuru:genericErrorHandlerFunc called")));
  char string__[5000];
  va_list argp__;
  va_start(argp__, msg__);
  vsnprintf(string__, 5000, msg__, argp__);
  FE_INFO(__F(("genericErrorHandlerFunc: Error: %1%") % string__));
  XSDGuru* guru_ = static_cast<XSDGuru*>(ctx__);
  guru_->validationErrors.push_back(string__);
  va_end(argp__);
}

bool XSDGuru::validateData(const char* data__)
{
  //clean out the validation errors & warnings
  validationErrors.clear();
  validationWarnings.clear();
   
  FE_TRACE(__F(("XSDGuru::validateData - validating data")));
  xmlDocPtr xmlDoc__ = NULL;
  xmlDocPtr schemaDoc__ = xsdParser.get_document()->cobj();

  xmlSchemaParserCtxtPtr parserCtxt__ = NULL;
  xmlSchemaPtr schema__ = NULL;
  xmlSchemaValidCtxtPtr validCtxt__ = NULL;
  FE_TRACE("XSDGuru::validateData - Preparse xsd document");

  xmlSetGenericErrorFunc(this, &genericErrorHandlerFunc);
  xmlSchemaSetValidErrors(validCtxt__, &xmlSchemaValidityErrorFunc, &xmlSchemaValidityWarningFunc, this);

  bool errorOccurred_ = false;
  if(!errorOccurred_ && !(parserCtxt__ = xmlSchemaNewDocParserCtxt(schemaDoc__)))
    {
      FE_ERROR(__F(("XSDGuru::validateData - pre-parse XSD document failed")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - Parse xsd document and create internal structure");
  if(!errorOccurred_ && !(schema__ = xmlSchemaParse(parserCtxt__)))
    {
      FE_ERROR(__F(("XSDGuru::validateData - parsing XSD document and creating internal structure failed")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - Create XSD context");
  if(!errorOccurred_ && !(validCtxt__ = xmlSchemaNewValidCtxt(schema__)))
    {
      FE_ERROR(__F(("XSDGuru::validateData - creating XSD context failed")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - Reading doc in memory");
  FE_TRACE(__F(("XSDGuru::validateData - byte array length is %1% \n\n%2%\n\n") % strlen(data__) % data__));
  if(!errorOccurred_ && !(xmlDoc__ = xmlReadMemory(data__, strlen(data__), NULL, NULL, XML_PARSE_XINCLUDE | XML_PARSE_NOBASEFIX)))
    {
      FE_ERROR(__F(("XSDGuru::validateData - reading document in memory failed")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - XInclude processing");
  if(!errorOccurred_ && (xmlXIncludeProcess(xmlDoc__) < 0))
    {
      FE_ERROR(__F(("XSDGuru::validateData - XInclude processing failed due to bad include file")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - Validating doc");
  if(!errorOccurred_ && (xmlSchemaValidateDoc(validCtxt__, xmlDoc__) != 0))
    {
      FE_ERROR(__F(("XSDGuru::validateData - document is invalid")));
      errorOccurred_ = true;
    }

  FE_TRACE("XSDGuru::validateData - Cleaning up data");

  if(validCtxt__) xmlSchemaFreeValidCtxt(validCtxt__);
  if(schema__) xmlSchemaFree(schema__);
  if(parserCtxt__) xmlSchemaFreeParserCtxt(parserCtxt__);
  if(xmlDoc__) xmlFreeDoc(xmlDoc__);

  FE_TRACE("XSDGuru::validateData - Finished validating");

  return !errorOccurred_;
}
