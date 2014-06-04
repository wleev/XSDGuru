#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/declaration_factory.hpp>
#include <tools/ssl/xsd/model/string_declaration.hpp>
#include <tools/ssl/xsd/model/int_declaration.hpp>
#include <tools/ssl/xsd/model/double_declaration.hpp>
#include <tools/ssl/xsd/model/bool_declaration.hpp>
#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <tools/ssl/xsd/model/type_declaration.hpp>
#include <tools/ssl/xsd/model/simple_content_declaration.hpp>
#include <tools/ssl/xsd/model/complex_content_declaration.hpp>

#include <boost/cast.hpp>

#include <iostream>
#include <climits>

using namespace std;
using namespace xmlpp;
using namespace boost;

map<string, ElementType> DeclarationFactory::extensionMap = {
   {"xs:string", ElementType::STRING},
   {"xs:ID", ElementType::STRING},
   {"xs:IDREF", ElementType::STRING},
   {"xs:language", ElementType::STRING},
   {"xs:Name", ElementType::STRING},
   {"xs:NCName", ElementType::STRING},
   {"xs:NMTOKEN", ElementType::STRING},
   {"xs:NMTOKENS", ElementType::STRING},
   {"xs:token", ElementType::STRING},
   {"xs:byte", ElementType::INT},
   {"xs:integer", ElementType::INT},
   {"xs:decimal", ElementType::DOUBLE},
   {"xs:boolean", ElementType::BOOL}
};

ElementDeclaration* DeclarationFactory::getDeclaration(Element* element__)
{
   ElementDeclaration* resultElement;
   string elementName_ = element__->get_attribute_value("name");
   NodeSet elementTypeSet = element__->find("@type");
   if(!elementTypeSet.empty())
   {
	  FE_TRACE(__F(("DeclarationFactory::getDeclaration - Getting declaration for element %1%") % elementName_));

      //we're getting an attribute type back, see xpath for @
	  Attribute* typeAttr = polymorphic_downcast<Attribute*>(elementTypeSet.front());
	  FE_TRACE(__F(("DeclarationFactory::getDeclaration - Element has type %1%") % typeAttr->get_value()));

	  //checking if type is a basic type defined by the xsd definition
	  //type should start with xs: if this is the namespace used for the xml schema definition
	  if(typeAttr->get_value().substr(0, 3) == "xs:")
	  {
		 resultElement = new ElementDeclaration(
			elementName_,
			elementName_,
			createSimpleBaseTypeDeclaration(typeAttr->get_value())
			);
	  } else {
		 NodeSet typeSet = element__->find("//*[@name='"+typeAttr->get_value()+"']");
		 //shouldn't have to check if empty, otherwise invalid xsd
		 Element* typeElement_ = polymorphic_downcast<Element*>(typeSet.front());
		 resultElement = createCustomTypeElementDeclaration(element__, typeElement_);
	  }
   } else
   {
	  FE_TRACE("DeclarationFactory::getDeclaration - Couldn't find attribute type so must be anonymous type");
	  //this means it's probably an anonymous type
	  //first element should be the type
	  //no children and no type attribute should invalid, so we can assume it has children
	  NodeSet nodeSet = element__->find("*");
	  Element* typeElement_ = polymorphic_downcast<Element*>(nodeSet.front());
	  FE_TRACE(__F(("DeclarationFactory::getDeclaration - Tag name of the first child is %1%") % typeElement_->get_name() ));
	  
	  resultElement = createCustomTypeElementDeclaration(element__, typeElement_);
   }

   int minOccurs = 0;
   int maxOccurs = 1;
   string minOccursString = element__->get_attribute_value("minOccurs");
   string maxOccursString = element__->get_attribute_value("maxOccurs");
   if(maxOccursString == "unbounded"){ maxOccurs = INT_MAX; }
   else if (!maxOccursString.empty()) { maxOccurs = atoi(maxOccursString.c_str());}
   minOccurs = atoi(minOccursString.c_str());

   
   FE_TRACE(__F(("DeclarationFactory::getDeclaration - Setting maxOccurs to %1%") % maxOccurs));
   FE_TRACE(__F(("DeclarationFactory::getDeclaration - Setting minOccurs to %1%") % minOccurs));
   resultElement->setMaxOccurs(maxOccurs);
   resultElement->setMinOccurs(minOccurs);
   return resultElement;
}

ElementDeclaration* DeclarationFactory::createCustomTypeElementDeclaration(Element* element__, Element* typeElement__)
{
   string elementName =  element__->get_attribute_value("name");   
   string tagName = typeElement__->get_name();
   string mixed = typeElement__->get_attribute_value("mixed");
   
   
   FE_TRACE(__F(("DeclarationFactory::getDeclaration - The element type is defined by us as a %1%") % tagName));
   
   if(tagName == "simpleType")
   {
	  return new ElementDeclaration(
		 elementName,
		 elementName,
		 createSimpleTypeDeclaration(typeElement__)
		 );
   } else if (tagName == "complexType")
   {
	  AttributeDeclarationSet attributes = extractAttributes(typeElement__);
	  ComplexElementDeclaration* result = createComplexDeclaration(typeElement__, elementName);
	  if(result->getContentType() == ContentType::GROUP){ result->addAttributes(attributes); }
	  return result;
   }
}


TypeDeclaration* DeclarationFactory::createSimpleTypeDeclaration(Element* typeElement__)
{
   FE_TRACE("DeclarationFactory::createSimpleTypeDeclaration - Creating simple type");
   //if this is an extension on a base type get the base type
   Attribute* baseTypeAttr = polymorphic_downcast<Attribute*>(typeElement__->find(".//@base").front());
   ElementType baseType = extensionMap[baseTypeAttr->get_value()];
   if(baseType ==  ElementType::STRING)
   {
	  return new StringDeclaration(typeElement__);
   } else if (baseType == ElementType::INT)
   {
	  return new IntDeclaration(typeElement__);
   } else if (baseType == ElementType::DOUBLE)
   {
	  return new DoubleDeclaration(typeElement__);
   } else if (baseType == ElementType::BOOL)
   {
	  return new BoolDeclaration();
   }
}

TypeDeclaration* DeclarationFactory::createSimpleBaseTypeDeclaration(string baseTypeName__)
{
   FE_TRACE("DeclarationFactory::createSimpleBaseTypeDeclaration - Creating simple base type");
   ElementType baseType = extensionMap[baseTypeName__];
   if(baseType ==  ElementType::STRING)
   {
	  return new StringDeclaration(baseTypeName__);
   } else if (baseType == ElementType::INT)
   {
	  return new IntDeclaration(baseTypeName__);
   } else if (baseType == ElementType::DOUBLE)
   {
	  return new DoubleDeclaration(baseTypeName__);
   } else if (baseType == ElementType::BOOL)
   {
	  return new BoolDeclaration();
   }
}

ComplexElementDeclaration* DeclarationFactory::createComplexDeclaration(Element* typeElement__, string elementName__)
{
   Element* firstElement = polymorphic_downcast<Element*>(typeElement__->find("*[1]").front());
   if(firstElement->get_name().find("simpleContent") != string::npos )
   {
	  FE_TRACE("DeclarationFactory::createComplexDeclaration - Creating simple content type");
	  return new SimpleContentDeclaration(typeElement__, elementName__);
   } else if( firstElement->get_name().find("complexContent") != string::npos)
   {
	  FE_TRACE("DeclarationFactory::createComplexDeclaration - Creating complex content type");
	  return new ComplexContentDeclaration(typeElement__, elementName__);
   } else
   {
	  FE_TRACE("DeclarationFactory::createComplexDeclaration - Creating group type");
	  return new GroupDeclaration(typeElement__, elementName__);
   }
}


AttributeDeclarationSet DeclarationFactory::extractAttributes(Element* element__)
{
   FE_TRACE(__F(("DeclarationFactory::extractAttributes - Extracting attributes for element %1%") % element__->get_name() ));
   
   NodeSet attributeSet = element__->find("*[local-name() = 'attribute']");
   AttributeDeclarationSet attributeResults;
   for(Node* n : attributeSet)
   {
	  Element* attributeElem = polymorphic_downcast<Element*>(n);
	  if(!attributeElem->get_attribute_value("ref").empty()){ continue; }
	  FE_TRACE(__F(("DeclarationFactory::extractAttributes - Extracting attribute %1%") % attributeElem->get_attribute_value("name") ));

	  TypeDeclaration* typeDeclaration_;
	  if(attributeElem->get_attribute_value("type").substr(0, 3) == "xs:")
	  {
		 typeDeclaration_ = createSimpleBaseTypeDeclaration(attributeElem->get_attribute_value("type"));
	  } else
	  {
		 NodeSet simpleTypeSet = element__->find("//*[@name = '"+attributeElem->get_attribute_value("type")+"']");
		 typeDeclaration_ = createSimpleTypeDeclaration(polymorphic_downcast<Element*>(simpleTypeSet.front()));
	  }
	  string name_ = attributeElem->get_attribute_value("name");
	  bool required_ = attributeElem->get_attribute_value("use") == "required" ? true : false;
	  bool fixed_ = attributeElem->get_attribute_value("fixed").empty() ? false : true;
	  string defaultValue_ = "";
	  if(fixed_){ defaultValue_ = attributeElem->get_attribute_value("fixed"); }
	  else{ defaultValue_ =  attributeElem->get_attribute_value("default"); }
	  attributeResults.push_back(
		 new AttributeDeclaration(name_, typeDeclaration_, required_, defaultValue_, fixed_)
		 );
   }
   return attributeResults;
}
