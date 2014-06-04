#ifndef NUMERIC_DECLARATION_HPP_
#define NUMERIC_DECLARATION_HPP_

#include <string>
#include <tools/ssl/xsd/model/base_declaration.hpp>
#include <boost/cast.hpp>

template<class numericType>
class NumericDeclaration : public BaseDeclaration<numericType>
{
   numericType max;
   numericType min;
   int totalDigits;
   
public:
   NumericDeclaration(std::string typeName__) :
	  BaseDeclaration<numericType>(typeName__){};
   virtual ~NumericDeclaration(){}

   virtual ElementType getType() = 0;
   numericType getMax(){ return max; }
   numericType getMin(){ return min; }
   int getTotalDigits(){ return totalDigits; }

   //utility xpath functions for extracting values
   std::string extractMin(xmlpp::Element* typeDeclaration__)
	  {
		 xmlpp::NodeSet minValueSet = typeDeclaration__->find(".//*[contains(local-name(), 'min')]/@value");
		 std::string min_;
		 if(!minValueSet.empty())
		 {
			xmlpp::Attribute* attr = boost::polymorphic_downcast<xmlpp::Attribute*>(minValueSet.front());
		    min_ = attr->get_value();
		 }
		 return min_;
	  }

   std::string extractMax(xmlpp::Element* typeDeclaration__)
	  {
		 xmlpp::NodeSet maxValueSet = typeDeclaration__->find(".//*[contains(local-name(), 'max')]/@value");
		 std::string max_;
		 if(!maxValueSet.empty())
		 {
			xmlpp::Attribute* attr = boost::polymorphic_downcast<xmlpp::Attribute*>(maxValueSet.front());
			max_ = attr->get_value();
		 }
		 return max_;
	  }

   std::string extractTotalDigits(xmlpp::Element* typeDeclaration__)
	  {
		 xmlpp::NodeSet totalDigitsValueSet = typeDeclaration__->find(".//*[local-name() = 'totalDigits']/@value");
		 std::string totalDigits_;
		 if(!totalDigitsValueSet.empty())
		 {
			xmlpp::Attribute* attr = boost::polymorphic_downcast<xmlpp::Attribute*>(totalDigitsValueSet.front());
			totalDigits_ = attr->get_value();
		 }
		 return totalDigits_;
	  }
   
   void setMax(numericType max__){ max = max__; }
   void setMin(numericType min__){ min = min__; }
   void setTotalDigits(int totalDigits__){ totalDigits = totalDigits__; }
};
#endif
