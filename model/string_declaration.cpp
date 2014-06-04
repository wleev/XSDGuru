#include <tools/ssl/xsd/model/string_declaration.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/cast.hpp>

#include <climits>

using namespace std;
using namespace xmlpp;
using namespace boost;

StringDeclaration::StringDeclaration(string baseType__) :
   BaseDeclaration(baseType__)
{
   setLength(INT_MAX);
   setMaxLength(INT_MAX);
   setMinLength(0);
}

StringDeclaration::StringDeclaration(Element* typeDeclaration__) :
   BaseDeclaration(typeDeclaration__->get_attribute_value("name"))
{
	//extract info with xpath's
   //enumeration
   setEnumValues(extractEnumValues(typeDeclaration__));
   
   //pattern
   setPattern(extractPattern(typeDeclaration__));

   setLength(extractLength(typeDeclaration__));
   setMaxLength(extractMaxLength(typeDeclaration__));
   setMinLength(extractMinLength(typeDeclaration__));
}

bool StringDeclaration::extractUnique(Element* typeDeclaration__)
{
//needs reworking, 
//    NodeSet pattern_value_set = TypeDeclaration__->find("//*[local-name() = '']/@value");
// 		 string pattern_;
// 		 if(!pattern_value_set.empty())
// 		 {
// 			Attribute* attr = polymorphic_downcast<Attribute*>(pattern_value_set.front());
// 			pattern_ = attr->get_value();
// 		 }
// 		 return pattern_;
}

int StringDeclaration::extractLength(Element* typeDeclaration__)
{
   NodeSet lengthValueSet = typeDeclaration__->find(".//*[local-name() = 'length']/@value");
   int length_;
   if(!lengthValueSet.empty())
   {
	  Attribute* attr = polymorphic_downcast<Attribute*>(lengthValueSet.front());
	  string tmp_ = attr->get_value();
	  try {
		 length_ = lexical_cast<int>(tmp_);
	  } catch(bad_lexical_cast const& ){
		 length_ = INT_MAX;
	  }
   }
   return length_;
}

int StringDeclaration::extractMaxLength(Element* typeDeclaration__)
{
   NodeSet lengthValueSet = typeDeclaration__->find(".//*[local-name() = 'maxLength']/@value");
   int length_;
   if(!lengthValueSet.empty())
   {
	  Attribute* attr = polymorphic_downcast<Attribute*>(lengthValueSet.front());
	  string tmp_ = attr->get_value();
	  try {
		 length_ = lexical_cast<int>(tmp_);
	  } catch(bad_lexical_cast const& ){
		 length_ = INT_MAX;
	  }
   }
   return length_;
}

int StringDeclaration::extractMinLength(Element* typeDeclaration__)
{
   NodeSet lengthValueSet = typeDeclaration__->find(".//*[local-name() = 'min_length']/@value");
   int length_;
   if(!lengthValueSet.empty())
   {
	  Attribute* attr = polymorphic_downcast<Attribute*>(lengthValueSet.front());
	  string tmp_ = attr->get_value();
	  try {
		 length_ = lexical_cast<int>(tmp_);
	  } catch(bad_lexical_cast const& ){
		 length_ = 0;
	  }
   }
   return length_;
}
