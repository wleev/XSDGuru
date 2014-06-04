#ifndef DOUBLE_DECLARATION_HPP_
#define DOUBLE_DECLARATION_HPP_
#include <vector>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/numeric_declaration.hpp>

class DoubleDeclaration : public NumericDeclaration<double>
{
   int precision;
public:
   DoubleDeclaration(std::string baseTypeName__);
   DoubleDeclaration(xmlpp::Element* typeDeclaration__);
   ~DoubleDeclaration(){}

   ElementType getType(){ return ElementType::DOUBLE; }
   int getPrecision(){ return precision; }

protected:
   void setPrecision(int precision_){ precision = precision_; }
};
#endif

