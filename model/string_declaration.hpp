#ifndef STRING_DECLARATION_HPP_
#define STRING_DECLARATION_HPP_
#include <vector>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/base_declaration.hpp>
#include <climits>


class StringDeclaration : public BaseDeclaration<std::string>
{
   bool unique = false;
   int length = INT_MAX;
   int maxLength = INT_MAX;
   int minLength = 0;
   
public:
   StringDeclaration(std::string baseType__);
   StringDeclaration(xmlpp::Element* typeDeclaration__);
   ~StringDeclaration(){}
   
   ElementType getType()
	  {
		 return ElementType::STRING;
	  }
   bool getUnique(){ return unique; }
   int getLength(){ return length; }
   int getMaxLength(){ return maxLength; }
   int getMinLength(){ return minLength; }

   void setUnique(bool unique_){ unique = unique_; }
   void setLength(int length_){ length = length_; }
   void setMaxLength(int length_){ maxLength = length_; }
   void setMinLength(int length_){ minLength = length_; }
   
protected:
   bool extractUnique(xmlpp::Element* type_declaration__);
   int extractLength(xmlpp::Element* type_declaration__);
   int extractMaxLength(xmlpp::Element* type_declaration__);
   int extractMinLength(xmlpp::Element* type_declaration__);
   
};

#endif

