#ifndef BASE_DECLARATION_HPP_
#define BASE_DECLARATION_HPP_

#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/type_declaration.hpp>

#include <boost/cast.hpp>



template<class baseType>
class BaseDeclaration : public TypeDeclaration
{
   std::vector<baseType> enumValues;
   std::string pattern;

public:
   BaseDeclaration(std::string typeName__) : TypeDeclaration(typeName__){}

   virtual ElementType getType() = 0;
   
   std::vector<baseType> getEnumValues(){ return enumValues; }
   std::string getPattern(){ return pattern; }

   //general xpath utility functions for extracting values
   std::vector<std::string> extractEnumValues(xmlpp::Element* typeDeclaration__)
	  {
		 xmlpp::NodeSet enumValueSet = typeDeclaration__->find(".//*[local-name() = 'enumeration' ]/@value");
		 std::vector<std::string> enumValues_;
		 if(!enumValueSet.empty())
		 {
			for(xmlpp::Node* n : enumValueSet)
			{
			   xmlpp::Attribute* attr = boost::polymorphic_downcast<xmlpp::Attribute*>(n);
			   enumValues_.push_back(attr->get_value());
			}
		 }
		 return enumValues_;
	  }

   std::string extractPattern(xmlpp::Element* typeDeclaration__)
	  {
		 xmlpp::NodeSet patternValueSet = typeDeclaration__->find(".//*[local-name() = 'pattern']/@value");
		 std::string pattern_;
		 if(!patternValueSet.empty())
		 {
			xmlpp::Attribute* attr = boost::polymorphic_downcast<xmlpp::Attribute*>(patternValueSet.front());
			pattern_ = attr->get_value();
		 }
		 return pattern_;
	  }
   
   void setEnumValues(std::vector<baseType> enum_values__){ enumValues = enum_values__; }
   void setPattern(std::string pattern__){ pattern = pattern__; }
};

#endif
