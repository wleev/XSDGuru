#ifndef ATTRIBUTE_DECLARATION_HPP_
#define ATTRIBUTE_DECLARATION_HPP_

#include <string>

class AttributeDeclaration : public NodeDeclaration
{
public:
   AttributeDeclaration(std::string name__, TypeDeclaration* typeDeclaration__, bool required__, std::string defaultValue__, bool fixed__ ) : NodeDeclaration(name__), typeDeclaration(typeDeclaration__), required(required__), defaultValue(defaultValue__), fixed(fixed__){}



   TypeDeclaration* getTypeDeclaration(){ return typeDeclaration; }
   bool isRequired(){ return required; }
   std::string getDefaultValue(){ return defaultValue; }
   bool isFixed(){ return fixed; }


private:
   TypeDeclaration* typeDeclaration;
   bool required;
   std::string defaultValue;
   bool fixed;
   
};

#endif
