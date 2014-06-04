#ifndef COMPLEX_ELEMENT_DECLARATION_HPP_
#define COMPLEX_ELEMENT_DECLARATION_HPP_

#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/type_declaration.hpp>
#include <tools/ssl/xsd/attribute_types.hpp>
#include <tools/ssl/xsd/type_enums.hpp>


class ComplexElementDeclaration : public ElementDeclaration
{
public:
   ComplexElementDeclaration(std::string name__, std::string displayString__, TypeDeclaration* typeDeclaration__):
	  ElementDeclaration(name__, displayString__, typeDeclaration__) {}
   ComplexElementDeclaration(std::string name__) : ElementDeclaration(name__) {}
   ~ComplexElementDeclaration(){}

   bool isComplex(){ return true;}

   virtual ContentType getContentType() = 0;

   void setAttributes(AttributeDeclarationSet attributes_){ attributes = attributes_; }
   AttributeDeclarationSet getAttributes(){ return attributes; }
   void addAttributes(AttributeDeclarationSet attributes);

private:
   AttributeDeclarationSet attributes;
};

#endif

