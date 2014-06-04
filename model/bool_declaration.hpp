#ifndef BOOL_DECLARATION_HPP_
#define BOOL_DECLARATION_HPP_

#include <vector>
#include <tools/ssl/xsd/model/base_declaration.hpp>

class BoolDeclaration : public BaseDeclaration<bool>
{
public:
   BoolDeclaration() : BaseDeclaration("bool"){}
   ~BoolDeclaration(){}

   ElementType getType(){ return ElementType::BOOL;}
};

#endif

