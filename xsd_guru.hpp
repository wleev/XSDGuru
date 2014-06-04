#ifndef XSD_GURU_HPP_
#define XSD_GURU_HPP_

#include <tools/ssl/xsd/types.hpp>
#include <tools/ssl/xsd/attribute_types.hpp>
#include <tools/ssl/xsd/type_enums.hpp>
#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <libxml++-2.6/libxml++/libxml++.h>

#include <memory>
#include <string>
#include <vector>
#include <map>

class XSDGuru
{
public:
   XSDGuru(std::string file__);
   ~XSDGuru();
   ElementDeclaration* getElementDeclaration(std::string elementName__);
   bool canHaveChildren(std::string elementName__);
   DeclarationSet rootNodes();
   AttributeDeclarationSet attributesForElement(std::string elementName__);
   DeclarationSet validChildrenForElement(std::string elementName__);
   DeclarationSet validChildrenForElement(std::string elementName__, std::vector<std::string> existingChildren__);
   std::vector<int> validPositionsForElement(std::string parentName__, std::string childName__, std::vector<std::string> existingChilden__);
   DeclarationSet validChildrenBeforeElement(std::string parentName__, int elementIndex__, std::vector<std::string> existingChildren__);
   DeclarationSet validChildrenAfterElement(std::string parentName__, int elementIndex__, std::vector<std::string> existingChildren__);
   std::vector<int> invalidIndexesAfterRemove(std::string parentName__, int removeIndex__, std::vector<std::string> existingChildren__ );
   bool validateData(const char* data__);
   std::vector<std::string> getValidationErrors(){ return validationErrors; }
   std::vector<std::string> getValidationWarnings(){ return validationWarnings; }
	
private:
   xmlpp::DomParser xsdParser;
   std::map<std::string, std::unique_ptr<ElementDeclaration> > declarationCache;
   std::vector<std::string> validationErrors;
   std::vector<std::string> validationWarnings;
   
   DeclarationSet validChildrenNextElementInGroup(GroupDeclaration* parentGroup__, int elementIndex__, std::vector<std::string> existingChildren__, std::function<int(int)> selector);
   DeclarationSet validChildrenNextElement(std::string parentName, int elementIndex, std::vector<std::string> existingChildren, std::function<int(int)> selector);
   bool isDeclarationAnonymousGroup(ElementDeclaration* element__);

   static void xmlSchemaValidityErrorFunc(void *ctx, const char *msg, ...);
   static void xmlSchemaValidityWarningFunc(void *ctx, const char *msg, ...);
   static void genericErrorHandlerFunc(void *ctx, const char *msg, ...);
};

#endif
