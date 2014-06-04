#ifndef SIMPLE_CONTENT_DECLARATION_HPP_
#define SIMPLE_CONTENT_DECLARATION_HPP_

#include <tools/ssl/xsd/model/complex_element_declaration.hpp>
#include <string>
#include <map>

class SimpleContentDeclaration : public ComplexElementDeclaration
{
   std::map<std::string, std::string> restrictions;
   
public:
   SimpleContentDeclaration(xmlpp::Element* typeElement__, std::string elementName__);

   ContentType getContentType(){ return ContentType::SIMPLE_CONTENT;}

private:
   TypeDeclaration* parseType(xmlpp::Element* typeElement__);
   TypeDeclaration* parseRestriction(xmlpp::Element* typeElement__);
   TypeDeclaration* parseExtension(xmlpp::Element* typeElement__);

   void mapRestrictions(xmlpp::Element* restrictionElement__);
   void configureRestrictions();
};

#endif
