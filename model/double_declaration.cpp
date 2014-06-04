#include <tools/ssl/xsd/model/double_declaration.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>

#include <cfloat>

using namespace std;
using namespace xmlpp;
using namespace boost;

DoubleDeclaration::DoubleDeclaration(string baseTypeName__) :
   NumericDeclaration<double>(baseTypeName__)
{
   setMax(DBL_MAX);
   setMax(DBL_MAX);
   setTotalDigits(DBL_DIG+DBL_MAX_10_EXP);
   setPrecision(DBL_DIG);
}

DoubleDeclaration::DoubleDeclaration(Element* typeDeclaration__) :
   NumericDeclaration<double>(typeDeclaration__->get_attribute_value("name"))
{
   //extract info with xpath's
   //enumeration
   vector<string> enumStrings_ = extractEnumValues(typeDeclaration__);
   vector<double> enumValues_;
   for(string s : enumStrings_ )
   {
	  try {
		 double tmp = lexical_cast<double>(s);
		 enumValues_.push_back(tmp);
	  } catch(bad_lexical_cast const& ){
		 continue;
	  }
   }
   setEnumValues(enumValues_);
   
   //pattern
   setPattern(extractPattern(typeDeclaration__));

   //min value
   string min_ = extractMin(typeDeclaration__);
   try {
	  double tmp = lexical_cast<double>(min_);
	  setMin(tmp);
   } catch(bad_lexical_cast const& ){
	  setMin(DBL_MIN);
   }
   
   //max value
   string max_ = extractMax(typeDeclaration__);
   try {
	  double tmp = lexical_cast<double>(max_);
	  setMax(tmp);
   } catch(bad_lexical_cast const& ){
	  setMax(DBL_MAX);
   }
   
   //total digits
   //probably won't be used just yet
   string totalDigits_ = extractTotalDigits(typeDeclaration__);
   try {
	  int tmp = lexical_cast<int>(totalDigits_);
	  setTotalDigits(tmp);
   } catch(bad_lexical_cast const& ){
	  setTotalDigits(DBL_DIG+DBL_MAX_10_EXP);
   }

   //double specific precision
   NodeSet precisionValueSet = typeDeclaration__->find(".//*[local-name() = 'fractionDigits']/@value");
   int precision_;
   if(!precisionValueSet.empty())
   {
	  Attribute* attr = polymorphic_downcast<Attribute*>(precisionValueSet.front());
	  try {
		 string tmp = attr->get_value();
		 precision_ = lexical_cast<int>(tmp);
		 setPrecision(precision_);
	  } catch(bad_lexical_cast const& ){
		 setPrecision(DBL_DIG);
	  }
   }
}
