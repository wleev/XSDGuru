#ifndef GROUP_DECLARATION_HPP_
#define GROUP_DECLARATION_HPP_

#include <tools/ssl/xsd/model/complex_element_declaration.hpp>
#include <tools/ssl/xsd/type_enums.hpp>
#include <tools/ssl/xsd/types.hpp>
#include <map>
#include <vector>

class GroupDeclaration : public ComplexElementDeclaration
{

public:
   GroupDeclaration(xmlpp::Element* element__, std::string elementName__);
   GroupDeclaration(xmlpp::Element* indicatorElement__, GroupDeclaration* indicatorParent__);
   GroupDeclaration(std::string elementName__) : ComplexElementDeclaration(elementName__) {}
   ~GroupDeclaration(){}
   ContentType getContentType(){ return ContentType::GROUP; }

   bool isAnonymous(){ return anonymous; }
   int getIndicatorMaxOccurs(){ return indicatorMaxOccurs; }
   int getIndicatorMinOccurs(){ return indicatorMinOccurs; }
   Indicator getIndicator(){ return indicator; }
   DeclarationSet getChildren(){ return childElements; }
   std::vector<std::string> getChildrenNames();
   GroupDeclaration* getParentGroup(){ return parentGroup; }

   ElementDeclaration* getChild(std::string elementName__);
   GroupDeclaration* getGroupForChild(std::string elementName__);

   
protected:
   ElementDeclaration* parseChild(xmlpp::Node* n__, GroupDeclaration* parent__);
   void parseIndicator(xmlpp::Element* element__, GroupDeclaration* parent__);

   Indicator indicator;
   DeclarationSet childElements;
   std::vector<std::string> childElementNames;
   int indicatorMaxOccurs = 1;
   int indicatorMinOccurs = 0;
   bool anonymous = false;

   //if anonymous
   GroupDeclaration* parentGroup;
};



#endif
