#ifndef NONE_TYPE_DECLARATION_
#define NONE_TYPE_DECLARATION_

#include <tools/ssl/xsd/model/type_declaration.hpp>

class NoneTypeDeclaration : public TypeDeclaration
{
public:
   NoneTypeDeclaration() : TypeDeclaration("None") {}
   ~NoneTypeDeclaration(){}

   ElementType getType(){ return ElementType::NONE; }
};

#endif
