#ifndef INT_DECLARATION_HPP_
#define INT_DECLARATION_HPP_
#include <vector>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/numeric_declaration.hpp>


class IntDeclaration : public NumericDeclaration<int>
{
public:
   IntDeclaration(std::string baseTypeName__);
   IntDeclaration(xmlpp::Element* typeDeclaration__);
   ~IntDeclaration(){}

   ElementType getType()
	  {
		 return ElementType::INT;
	  }
};

#endif

