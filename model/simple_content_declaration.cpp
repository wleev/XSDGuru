#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/model/simple_content_declaration.hpp>
#include <tools/ssl/xsd/declaration_factory.hpp>
#include <tools/ssl/xsd/model/string_declaration.hpp>
#include <tools/ssl/xsd/model/double_declaration.hpp>
#include <tools/ssl/xsd/model/int_declaration.hpp>
#include <tools/ssl/xsd/model/bool_declaration.hpp>

#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <iterator>
#include <vector>
#include <set>

using namespace xmlpp;
using namespace std;
using namespace boost;

SimpleContentDeclaration::SimpleContentDeclaration(Element* typeElement__, string elementName__) :
   ComplexElementDeclaration(elementName__)
{
   typeDeclaration = parseType(typeElement__);
   displayString = elementName__;

   configureRestrictions();
}

TypeDeclaration* SimpleContentDeclaration::parseType(Element* typeElement__)
{
   Element* simpleElement;
   if(typeElement__->get_name().find("simpleType") == string::npos)
   {
	  NodeSet simpleContentSet = typeElement__->find("*[local-name() = 'simpleContent']");
	  simpleElement = polymorphic_downcast<Element*>(simpleContentSet.front());
   } else
   {
	  simpleElement = typeElement__;
   }
   NodeSet directChildrenSet = simpleElement->find("*[local-name() = 'extension' or local-name() = 'restriction']");
   Element* firstChild = polymorphic_downcast<Element*>(directChildrenSet.front());

   // string baseType = firstChild->get_attribute_value("base");
   // while(baseType.find("xs:") == string::npos)
   // {
   // 	  Element* tempType = polymorphic_downcast<Element*>(typeElement__->find("/*[@name = '"+baseType+"']").front());
   // 	  baseType = tempType->get_attribute_value("base");
   // }

   // DeclarationFactory::createSimpleBaseTypeDeclaration(baseType);
   
   if(firstChild->get_name().find("extension") != string::npos ){
	  FE_TRACE("SimpleContentDeclaration::parseType - Found extension");
	  return parseExtension(firstChild);
   } else{
	  FE_TRACE("SimpleContentDeclaration::parseType - Found restriction");
	  return parseRestriction(firstChild);
   }
}

TypeDeclaration* SimpleContentDeclaration::parseExtension(Element* extensionElement__){
   string baseType = extensionElement__->get_attribute_value("base");
   FE_TRACE(__F(("SimpleContentDeclaration::parseExtension - Parsing extension with basetype %1%") % baseType));
   Element* baseTypeElement;

   //add the attributes in this extension, if none available then question the user's xsd skills
   AttributeDeclarationSet attributes_ = DeclarationFactory::extractAttributes(extensionElement__);
   if(attributes_.empty()){
	  //throw AreYouSureYouKnowHowToWriteXSDException("Are you really sure?");
	  FE_WARN("SimpleContentDeclaration::parseExtension - Found simpleContent extension with no attributes, are you sure you should be using an extension?");
   }
   addAttributes(attributes_);
   
   if(baseType.empty()){
	  //Anonymous simpleType or invalid xsd
	  NodeSet directChildrenSet = extensionElement__->find("*[local-name() = 'simpleType']");
	  if(!directChildrenSet.front()){
		 /* throw exception or something, invalid xsd */
		 FE_ERROR("SimpleContentDeclaration::parseExtension - Are you sure this is a valid XSD?");
	  }
	  baseTypeElement = polymorphic_downcast<Element*>(directChildrenSet.front());
   } else {
	  if(baseType.substr(0, 3) == "xs:"){
		 //it's a basic type, return its typedecl
		 NodeSet attributes = extensionElement__->find("*[local-name() = 'attribute']");
		 return DeclarationFactory::createSimpleBaseTypeDeclaration(baseType);		 
	  }
	  baseTypeElement = polymorphic_downcast<Element*>(extensionElement__->find("/*[local-name() = 'simpleType' and @name = '"+baseType+"']").front());
   }
   return parseType(baseTypeElement);
}


TypeDeclaration* SimpleContentDeclaration::parseRestriction(Element* restrictionElement__)
{
   string baseType = restrictionElement__->get_attribute_value("base");
   FE_TRACE(__F(("SimpleContentDeclaration::parseRestriction - Parsing restriction with basetype %1%") % baseType));
   Element* baseTypeElement;

   mapRestrictions(restrictionElement__);
   
   if(baseType.empty()){
	  FE_TRACE("SimpleContentDeclaration::parseRestriction - Base not found, must be anonymous base type.");
	  //Anonymous simpleType or invalid xsd
	  NodeSet directChildrenSet = restrictionElement__->find("*[local-name() = 'simpleType']");
	  if(!directChildrenSet.front()){
		 /* throw exception or something, invalid xsd */
		 FE_ERROR("SimpleContentDeclaration::parseExtension - Are you sure this is a valid XSD?");
	  }
	  baseTypeElement = polymorphic_downcast<Element*>(directChildrenSet.front());
   } else {
	  if(baseType.substr(0, 3) == "xs:"){
		 //it's a basic type, return its typedecl
		 return DeclarationFactory::createSimpleBaseTypeDeclaration(baseType);		 
	  }
	  NodeSet baseTypeSet = restrictionElement__->find("//*[@name = '"+baseType+"']");
	  if(baseTypeSet.empty())
	  {
		 FE_ERROR(__F(("SimpleContentDeclaration::parseRestriction - Couldn't find the type with name %1%") % baseType));
	  }
	  baseTypeElement = polymorphic_downcast<Element*>(baseTypeSet.front());
   }
   return parseType(baseTypeElement);		  
}


void SimpleContentDeclaration::mapRestrictions(Element* restrictionElement__)
{
   FE_TRACE("SimpleContentDeclaration::mapRestrictions - Indicated restrictions are being saved.");
   NodeSet enumerationSet = restrictionElement__->find("*[local-name() = 'enumeration']");
   if(restrictions.count("enumeration") == 0 && !enumerationSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found enumeration restriction.");
	  string enumResult;
	  for(int i = 0; i < enumerationSet.size(); i++)
	  {
		 Element* enumEl = polymorphic_downcast<Element*>(enumerationSet[i]);
		 string value = enumEl->get_attribute_value("value");
		 if(value.empty()){
			NodeSet valueSet = enumEl->find("*");
			for(int j = 0; j < valueSet.size(); i++)
			{
			   Element* valueEl = polymorphic_downcast<Element*>(valueSet[j]);
			   enumResult += valueEl->get_child_text()->get_content();
			   if(i != enumerationSet.size() - 1 && j != valueSet.size() - 1)
			   {
				  enumResult += "&";
			   }
			}
		 } else
		 {
			enumResult += value;
			if(i != enumerationSet.size() - 1)
			{
			   enumResult += "&";
			}
		 }
	  }
	  restrictions["enumeration"]= enumResult;
   }
   
   NodeSet patternSet = restrictionElement__->find("*[local-name() = 'pattern']");
   if(restrictions.count("pattern") == 0 && !patternSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found pattern restriction.");
	  string patternResult;
	  for(int i = 0; i < patternSet.size(); i++)
	  {
		 Element* patternEl = polymorphic_downcast<Element*>(patternSet[i]);
		 patternResult += patternEl->get_attribute_value("value");
		 if(i != patternSet.size() - 1)
		 {
			patternResult += "|";
		 }
	  }
	  restrictions["pattern"] = patternResult;
   }

   //numeric part
   NodeSet maxSet = restrictionElement__->find("*[local-name() = 'maxInclusive' or local-name() = 'maxExclusive']");
   if(restrictions.count("max") == 0 && !maxSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found max(In/Ex)clusive restriction.");
	  Element* maxEl = polymorphic_downcast<Element*>(maxSet.front());
	  restrictions["max"] = maxEl->get_attribute_value("value");
   }

   NodeSet minSet = restrictionElement__->find("*[local-name() = 'minInclusive' or local-name() = 'minExclusive']");
   if(restrictions.count("min") == 0 && !minSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found min(In/Ex)clusive restriction.");
	  Element* minEl = polymorphic_downcast<Element*>(minSet.front());
	  restrictions["min"] = minEl->get_attribute_value("value");
   }
   
   NodeSet tdSet = restrictionElement__->find("*[local-name() = 'totalDigits']");
   if(restrictions.count("totalDigits") == 0 && !tdSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found totalDigits restriction.");
	  Element* tdEl = polymorphic_downcast<Element*>(tdSet.front());
	  restrictions["totalDigits"] = tdEl->get_attribute_value("value");
   }

   NodeSet fdSet = restrictionElement__->find("*[local-name() = 'fractionDigits']");
   if(restrictions.count("fractionDigits") == 0 && !fdSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found fractionDigits restriction.");
	  Element* fdEl = polymorphic_downcast<Element*>(fdSet.front());
	  restrictions["fractionDigits"] = fdEl->get_attribute_value("value");
   }

   //string part
   NodeSet lengthSet = restrictionElement__->find("*[local-name() = 'length']");
   if(restrictions.count("length") == 0 && !lengthSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found length restriction.");
	  Element* lengthEl = polymorphic_downcast<Element*>(lengthSet.front());
	  restrictions["length"] = lengthEl->get_attribute_value("value");
   }

   NodeSet maxLengthSet = restrictionElement__->find("*[local-name() = 'maxLength']");
   if(restrictions.count("maxLength") == 0 && !maxLengthSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found maxLength restriction.");
	  Element* lengthEl = polymorphic_downcast<Element*>(maxLengthSet.front());
	  restrictions["maxLength"] = lengthEl->get_attribute_value("value");
   }

   NodeSet minLengthSet = restrictionElement__->find("*[local-name() = 'minLength']");
   if(restrictions.count("minLength") == 0 && !minLengthSet.empty())
   {
	  FE_TRACE("SimpleContentDeclaration::mapRestrictions - Found minLength restriction.");
	  Element* lengthEl = polymorphic_downcast<Element*>(minLengthSet.front());
	  restrictions["minLength"] = lengthEl->get_attribute_value("value");
   }
}


void SimpleContentDeclaration::configureRestrictions()
{
   vector<string> enumValues;
   //base restrictions
   if(restrictions.count("enumerations") > 0)
   {
	  boost::split(enumValues, restrictions["enumeration"], boost::is_any_of("&"));
   }

   string patternValue = restrictions.count("pattern") > 0 ? restrictions["pattern"] : "";
   
   if(typeDeclaration->getType() == ElementType::STRING)
   {
	  StringDeclaration* stringDecl = polymorphic_downcast<StringDeclaration*>(typeDeclaration);
	  stringDecl->setEnumValues(enumValues);
	  stringDecl->setPattern(patternValue);

	  if(restrictions.count("length") > 0) { stringDecl->setLength(boost::lexical_cast<int>(restrictions["length"])); }
	  if(restrictions.count("maxLength") > 0) { stringDecl->setMaxLength(boost::lexical_cast<int>(restrictions["maxLength"])); }
	  if(restrictions.count("minLength") > 0) { stringDecl->setMinLength(boost::lexical_cast<int>(restrictions["minLength"])); }
   }

   if(typeDeclaration->getType() == ElementType::INT)
   {
	  IntDeclaration* intDecl = polymorphic_downcast<IntDeclaration*>(typeDeclaration);
	  intDecl->setPattern(patternValue);

	  vector<int> intEnumValues;
	  for(string s : enumValues){ intEnumValues.push_back(boost::lexical_cast<int>(s)); }
	  intDecl->setEnumValues(intEnumValues);

	  if(restrictions.count("max") > 0) { intDecl->setMax(boost::lexical_cast<int>(restrictions["max"])); }
		 if(restrictions.count("min") > 0) { intDecl->setMin(boost::lexical_cast<int>(restrictions["min"])); }
			if(restrictions.count("totalDigits") > 0) { intDecl->setTotalDigits(boost::lexical_cast<int>(restrictions["totalDigits"])); }  
   }

   if(typeDeclaration->getType() == ElementType::DOUBLE)
   {
	  DoubleDeclaration* doubleDecl = polymorphic_downcast<DoubleDeclaration*>(typeDeclaration);
	  doubleDecl->setPattern(patternValue);

	  vector<double> doubleEnumValues;
	  for(string s : enumValues){ doubleEnumValues.push_back(boost::lexical_cast<double>(s)); }
	  doubleDecl->setEnumValues(doubleEnumValues);

	  if(restrictions.count("max") > 0) { doubleDecl->setMax(boost::lexical_cast<double>(restrictions["max"])); }
		 if(restrictions.count("min") > 0) { doubleDecl->setMin(boost::lexical_cast<double>(restrictions["min"])); }
			if(restrictions.count("totalDigits") > 0) { doubleDecl->setTotalDigits(boost::lexical_cast<int>(restrictions["totalDigits"])); }
			   if(restrictions.count("fractionDigits") > 0) { doubleDecl->setTotalDigits(boost::lexical_cast<int>(restrictions["fractionDigits"])); }
   }
   
}
