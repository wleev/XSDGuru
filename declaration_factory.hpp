#ifndef DECLARATION_FACTORY_HPP_
#define DECLARATION_FACTORY_HPP_

#include <libxml++-2.6/libxml++/libxml++.h>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/complex_element_declaration.hpp>
#include <tools/ssl/xsd/model/type_declaration.hpp>
#include <tools/ssl/xsd/type_enums.hpp>
#include <tools/ssl/xsd/types.hpp>
#include <tools/ssl/xsd/attribute_types.hpp>
#include <map>

class DeclarationFactory
{
public:
   static ElementDeclaration* getDeclaration(xmlpp::Element* element__);
   static TypeDeclaration* createSimpleBaseTypeDeclaration(std::string baseTypeName__);
   static AttributeDeclarationSet extractAttributes(xmlpp::Element* element__);
private:
   static ComplexElementDeclaration* createComplexDeclaration(xmlpp::Element* element__, std::string elementName__);
   static TypeDeclaration* createSimpleTypeDeclaration(xmlpp::Element* typeElement__);
   static ElementDeclaration* createCustomTypeElementDeclaration(xmlpp::Element* element__, xmlpp::Element* typeElement__);
   static std::map<std::string, ElementType> extensionMap;
};
#endif
