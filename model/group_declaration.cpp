#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <tools/ssl/xsd/types.hpp>
#include <tools/ssl/xsd/declaration_factory.hpp>
#include <tools/ssl/xsd/model/none_type_declaration.hpp>

#include <boost/cast.hpp>

#include <map>
#include <iostream>
#include <climits>

using namespace std;
using namespace xmlpp;
using namespace boost;

GroupDeclaration::GroupDeclaration(Element* typeElement__, string elementName__) :
   ComplexElementDeclaration(elementName__, elementName__, new NoneTypeDeclaration())
{
   typeName = typeElement__->get_attribute_value("name");
   
   FE_TRACE(__F(("GroupDeclaration::GroupDeclaration - Constructor called for %1% with type %2%") % elementName__ %typeName));

   if(typeElement__->find("*[local-name() = 'all' or local-name() = 'sequence' or local-name() = 'choice']").empty())
   {
	  FE_ERROR("GroupDeclaration::GroupDeclaration - Type element did not have an indicator (choice/sequence/all) child ");
   } else {
	  Element* indicatorElement  = polymorphic_downcast<Element*>(typeElement__->find("*[local-name() = 'all' or local-name() = 'sequence' or local-name() = 'choice']").front());
   
	  parseIndicator(indicatorElement, this);
   }
}

GroupDeclaration::GroupDeclaration(Element* indicatorElement__, GroupDeclaration* parent__) :
   ComplexElementDeclaration(indicatorElement__->get_name())
{
   parseIndicator(indicatorElement__, parent__);
}

void GroupDeclaration::parseIndicator(Element* indicatorElement__, GroupDeclaration* parent__)
{
   if(indicatorElement__->get_name() == "sequence")
   {
	  FE_TRACE("GroupDeclaration::GroupDeclaration - Indicator is a sequence");
	  indicator = Indicator::SEQUENCE;
   } else if(indicatorElement__->get_name() == "all")
   {
	  FE_TRACE("GroupDeclaration::GroupDeclaration - Indicator is an all");
	  indicator = Indicator::ALL;
   } else if(indicatorElement__->get_name() == "choice")
   {
	  FE_TRACE("GroupDeclaration::GroupDeclaration - Indicator is a choice");
	  indicator = Indicator::CHOICE;
   }

   string maxOccursValue_ = indicatorElement__->get_attribute_value("maxOccurs");
   string minOccursValue_ = indicatorElement__->get_attribute_value("minOccurs");

   if(maxOccursValue_ == "unbounded"){ indicatorMaxOccurs= INT_MAX; }
   else if(!maxOccursValue_.empty()){ indicatorMaxOccurs = atoi(maxOccursValue_.c_str());}
   if(!minOccursValue_.empty()){ indicatorMinOccurs = atoi(minOccursValue_.c_str()); }
   
   
   NodeSet allChildren = indicatorElement__->find("*");
   for(Node* n : allChildren)
   {
	  ElementDeclaration* decl = parseChild(n, parent__);
	  childElementNames.push_back( decl->getName() );
	  childElements.push_back( decl );
   }
}

ElementDeclaration* GroupDeclaration::parseChild(Node* n__, GroupDeclaration* parent__)
{
   Element* tmp = polymorphic_downcast<Element*>(n__);
   if(tmp){
	  string tagName_ = tmp->get_name();
	  string elementName_ = tmp->get_attribute_value("name");
	  string typeName_ = tmp->get_attribute_value("type");
	  ElementDeclaration* decl;
	  FE_TRACE(__F(("GroupDeclaration::GroupDeclaration - Adding child %1% with type %2%") % elementName_ % typeName_));
	  FE_TRACE(__F(("GroupDeclaration::GroupDeclaration - Parent %1% has type %2%") % parent__->getName() % parent__->getTypeName()));
	  if(tagName_.find("choice") != string::npos ||
		 tagName_.find("sequence") != string::npos ||
		 tagName_.find("all") != string::npos)
	  {
		 GroupDeclaration* tempGroup = new GroupDeclaration(tmp, parent__);
		 tempGroup->anonymous = true;
		 tempGroup->parentGroup = parent__;
		 decl = tempGroup;
		 
	  } else if(typeName_ == parent__->getTypeName() ){ decl = parent__;}
	  else { decl = DeclarationFactory::getDeclaration(tmp); }
	  return decl;
   }
}

ElementDeclaration* GroupDeclaration::getChild(string elementName__)
{
   FE_TRACE(__F(("GroupDeclaration::getChild - Getting child %1%") % elementName__));
   auto child_ = find_if(
	  childElements.begin(),
	  childElements.end(),
	  [&](ElementDeclaration* e){ return e->getName() == elementName__; }
	  );
   if(child_ != childElements.end()){ return *child_; }
   else {
	  for(ElementDeclaration* c : childElements)
	  {
		 FE_TRACE(__F(("GroupDeclaration::getChild - Checking if child %1% has other children") % c->getName()));
		 if(c->isComplex())
		 {
			ComplexElementDeclaration* complexEl = polymorphic_downcast<ComplexElementDeclaration*>(c);
			if(complexEl->getContentType() == ContentType::GROUP)
			{
			   FE_TRACE(__F(("GroupDeclaration::getChild - Checking children of %1%") % complexEl->getName()));
			   GroupDeclaration* groupEl = polymorphic_downcast<GroupDeclaration*>(complexEl);
			   ElementDeclaration* tempRes = groupEl->getChild(elementName__);
			   if(tempRes){ return tempRes; }
			}
		 }
	  }
   }
   return 0;
}


vector<string> GroupDeclaration::getChildrenNames()
{
   FE_TRACE("GroupDeclaration::getChild - Getting children names");

   vector<string> names_;
   
   for(ElementDeclaration* c : childElements)
   {
	  bool canAdd = true;
	  FE_TRACE(__F(("GroupDeclaration::getChild - Checking if child %1% has other children") % c->getName()));
	  if(c->isComplex())
	  {
		 ComplexElementDeclaration* complexEl = polymorphic_downcast<ComplexElementDeclaration*>(c);
		 if(complexEl->getContentType() == ContentType::GROUP)
		 {
			FE_TRACE(__F(("GroupDeclaration::getChild - Adding children names of %1%") % complexEl->getName()));
			GroupDeclaration* groupEl = polymorphic_downcast<GroupDeclaration*>(complexEl);
			if(groupEl->isAnonymous())
			{
			   canAdd = false;
			   for(string tmpName : groupEl->getChildrenNames())
			   {
				  names_.push_back(tmpName);
			   }
			}
		 }
	  }
	  if(canAdd){ names_.push_back(c->getName());}
   }
   
   return names_;
}


GroupDeclaration* GroupDeclaration::getGroupForChild(string elementName__)
{
   FE_TRACE(__F(("GroupDeclaration::getGroupForChild - Getting group for child %1%") % elementName__));
   auto child_ = find_if(
	  childElements.begin(),
	  childElements.end(),
	  [&](ElementDeclaration* e){ return e->getName() == elementName__; }
	  );
   if(child_ != childElements.end()){ return this; }
   else {
	  for(ElementDeclaration* c : childElements)
	  {
		 FE_TRACE(__F(("GroupDeclaration::getChild - Checking if child %1% has other children") % c->getName()));
		 if(c->isComplex())
		 {
			ComplexElementDeclaration* complexEl = polymorphic_downcast<ComplexElementDeclaration*>(c);
			if(complexEl->getContentType() == ContentType::GROUP)
			{
			   FE_TRACE(__F(("GroupDeclaration::getChild - Checking children of %1%") % complexEl->getName()));
			   GroupDeclaration* groupEl = polymorphic_downcast<GroupDeclaration*>(complexEl);
			   ElementDeclaration* tempRes = groupEl->getChild(elementName__);
			   if(tempRes){ return groupEl; }
			}
		 }
	  }
   }
   return 0;
}
