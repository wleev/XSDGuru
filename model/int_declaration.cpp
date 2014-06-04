#include <tools/ssl/xsd/model/int_declaration.hpp>
#include <boost/lexical_cast.hpp>
#include <climits>

using namespace std;
using namespace xmlpp;
using namespace boost;

IntDeclaration::IntDeclaration(string baseTypeName__) :
   NumericDeclaration<int>(baseTypeName__)
{
   setMin(INT_MIN);
   setMax(INT_MAX);
   setTotalDigits(10);
}

IntDeclaration::IntDeclaration(Element* typeDeclaration__) :
   NumericDeclaration<int>(typeDeclaration__->get_attribute_value("name"))
{
   //extract info with xpath's
   //enumeration
   vector<string> enumStrings_ = extractEnumValues(typeDeclaration__);
   vector<int> enumValues_;
   for(string s : enumStrings_ )
   {
	  try {
		 int tmp = lexical_cast<int>(s);
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
	  int tmp = lexical_cast<int>(min_);
	  setMin(tmp);
   } catch(bad_lexical_cast const& ){
	  setMin(INT_MIN);
   }
   
   //max value
   string max_ = extractMax(typeDeclaration__);
   try {
	  int tmp = lexical_cast<int>(max_);
	  setMax(tmp);
   } catch(bad_lexical_cast const& ){
	  setMax(INT_MAX);
   }
   
   //total digits
   //probably won't be used just yet
   string totalDigits_ = extractTotalDigits(typeDeclaration__);
   try {
	  int tmp = lexical_cast<int>(totalDigits_);
	  setTotalDigits(tmp);
   } catch(bad_lexical_cast const& ){
	  setTotalDigits(10);
   }
}
