#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/model/complex_content_declaration.hpp>
#include <tools/ssl/xsd/declaration_factory.hpp>

#include <boost/cast.hpp>

using namespace std;
using namespace xmlpp;
using namespace boost;

ComplexContentDeclaration::ComplexContentDeclaration(Element* element__, string elementName__) :
   GroupDeclaration(elementName__)
{
   setTypeName(element__->get_attribute_value("name"));
   parseType(element__);
}


void ComplexContentDeclaration::parseType(Element* element__)
{
   Element* complexContentElement =
	  polymorphic_downcast<Element*>(
		 element__->find("*[local-name() = 'complexContent']").front()
		 );
   
   //complexContent child is either restriction or extension
   Element* child =
	  polymorphic_downcast<Element*>(
		 complexContentElement->find("*[1]").front()
		 );

   string childType = child->get_name();
   if( childType.find("restriction") != string::npos )
   {
	  parseRestriction(child);
   } else if( childType.find("extension") != string::npos )
   {
	  //extension elements are always joined under a sequence with minoccurs 1 and maxoccurs 1
	  indicator = Indicator::SEQUENCE;
	  indicatorMaxOccurs = 1;
	  indicatorMinOccurs = 1;
   
	  parseExtension(child);
   }   
}


void ComplexContentDeclaration::parseRestriction(Element* restrictionElement__)
{
   Element* indicatorElement  = polymorphic_downcast<Element*>(restrictionElement__->find("*[local-name() = 'all' or local-name() = 'sequence' or local-name() = 'choice']").front());
   parseIndicator(indicatorElement, this);
}

void ComplexContentDeclaration::parseExtension(Element* extensionElement__)
{
   //base elements come before extension elements in the sequence
   string baseTypeName = extensionElement__->get_attribute_value("base");
   FE_TRACE(__F(("ComplexContentDeclaration::parseRestriction - Parsing extension with base %1%") % baseTypeName));
   NodeSet baseTypeSet = extensionElement__->find("//*[@name ='"+baseTypeName+"']");
   if(baseTypeSet.empty())
   {
	  FE_ERROR("ComplexContentDeclaration::parseRestriction - Couldn't find the base complexType element, is this a valid XSD?");
   }
   Element* baseType = polymorphic_downcast<Element*>(baseTypeSet.front());
   NodeSet baseChildSet = baseType->find("*[1]");
   if(baseChildSet.empty())
   {
	  FE_ERROR("ComplexContentDeclaration::parseRestriction - The base complexType did not have any children, is this a valid XSD?");
   }
   Element* firstElementBase = polymorphic_downcast<Element*>(baseChildSet.front());
   if(firstElementBase->get_name().find("complexContent") == string::npos)
   {
	  FE_TRACE(__F(("ComplexContentDeclaration::parseRestriction - Base %1% is not a complexContent type") % baseTypeName));
	  string firstChildName = firstElementBase->get_name();
	  if(firstChildName.find("sequence") != string::npos ||
		 firstChildName.find("all") != string::npos ||
		 firstChildName.find("choice") != string::npos)
	  {
		 childElements.push_back( parseChild(firstElementBase, this) );
	  }
	  AttributeDeclarationSet attributes_ = DeclarationFactory::extractAttributes(baseType);
	  if(!attributes_.empty()){ addAttributes(attributes_); }
   } else
   {
	  parseType(baseType);
   }
   NodeSet childElements_ = extensionElement__->find("*");
   for(Node* n : childElements_)
   {
	  ElementDeclaration* decl = parseChild(n, this);
	  childElements.push_back( decl );
   }
   
   AttributeDeclarationSet attributes_ = DeclarationFactory::extractAttributes(extensionElement__);
   if(!attributes_.empty()){ addAttributes(attributes_); }
}

