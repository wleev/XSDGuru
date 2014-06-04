#define FE_DLOG_LEVEL FE_LOG_INFO
#include <common/logger/logger.hpp>

#include <tools/ssl/xsd/model/complex_element_declaration.hpp>
#include <algorithm>

void ComplexElementDeclaration::addAttributes(AttributeDeclarationSet attributes__){
   for(AttributeDeclaration* attr : attributes__)
   {
	  int check = count_if(
		 attributes.begin(),
		 attributes.end(),
		 [&](AttributeDeclaration * a ){ return a->getName() == attr->getName();}
		 );
	  if(check == 0){
		 FE_TRACE(__F(("ComplexElementDeclaration::addAttributes - Adding attribute %1% to the list") % attr->getName()));
		 attributes.push_back(attr);
	  }
   }
}

