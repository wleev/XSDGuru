#ifndef COMPLEX_CONTENT_DECLARATION_HPP_
#define COMPLEX_CONTENT_DECLARATION_HPP_

#include <tools/ssl/xsd/model/group_declaration.hpp>

class ComplexContentDeclaration : public GroupDeclaration
{
public:
   ComplexContentDeclaration(xmlpp::Element* element__, std::string elementName__);

   //ContentType getContentType(){ return ContentType::COMPLEX_CONTENT;}
   
private:
   void parseType(xmlpp::Element* element__);
   void parseRestriction(xmlpp::Element* element__);
   void parseExtension(xmlpp::Element* element__);
};

#endif
