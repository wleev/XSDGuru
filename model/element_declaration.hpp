#ifndef ELEMENT_DECLARATION_HPP_
#define ELEMENT_DECLARATION_HPP_
#include <string>
#include <libxml++-2.6/libxml++/libxml++.h>
#include <tools/ssl/xsd/model/node_declaration.hpp>
#include <tools/ssl/xsd/model/type_declaration.hpp>

class ElementDeclaration : public NodeDeclaration
{
public:
   ElementDeclaration(std::string name__, std::string displayString__, TypeDeclaration* typeDeclaration__): NodeDeclaration(name__), displayString(displayString__), typeDeclaration(typeDeclaration__) {}
   ElementDeclaration(std::string name__) : NodeDeclaration(name__), typeDeclaration(nullptr) {}
   ~ElementDeclaration(){}

   virtual bool isComplex(){ return false; }

   TypeDeclaration* getTypeDeclaration(){ return typeDeclaration;}
   std::string getDisplayString(){ return displayString;}
   void setDisplayString(std::string displayString__){ displayString == displayString__;}
   int getMaxOccurs(){ return maxOccurs; }
   int getMinOccurs(){ return minOccurs;}
   void setMinOccurs(int min__){ minOccurs = min__;}
   void setMaxOccurs(int max__){ maxOccurs = max__;}
   
protected:
   TypeDeclaration* typeDeclaration;
   std::string displayString;
   int maxOccurs = 1;
   int minOccurs = 0;
};

#endif










