#define FE_DLOG_LEVEL FE_LOG_TRACE
#include <common/logger/logger.hpp>

#include <unittest++/UnitTest++.h>
#include <unittest++/TestReporterStdout.h>

#include <tools/ssl/xsd/xsd_guru.hpp>
#include <tools/ssl/xsd/model/element_declaration.hpp>
#include <tools/ssl/xsd/model/group_declaration.hpp>
#include <tools/ssl/xsd/model/simple_content_declaration.hpp>
#include <tools/ssl/xsd/model/complex_content_declaration.hpp>
#include <tools/ssl/xsd/model/int_declaration.hpp>
#include <tools/ssl/xsd/model/string_declaration.hpp>
#include <tools/ssl/xsd/regex_guru.hpp>

#include <boost/regex.hpp>

#include <re2/re2.h>

#include <fstream>
#include <iostream>
#include <list>
#include <algorithm>
#include <climits>
#include <regex>

using namespace std;
using namespace UnitTest;

TEST(ElementTypeTest)
{
   FE_INFO("/////////  ElementTypeTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");
	  
	  CHECK(type->isComplex());
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ElementTypeTest end ////////////");
}

TEST(AnonymousTypeTest)
{
   FE_INFO("/////////  AnonymousTypeTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_AnonymousTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");

	  CHECK(type->isComplex());
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }	
   FE_INFO("/////////  AnonymousTypeTest end ////////////");
}

TEST(ElementCanHaveChildrenTest)
{
   FE_INFO("/////////  ElementCanHaveChildrenTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_AnonymousTypeTest.xml");
	  bool canHaveChildren = guru->canHaveChildren("region");
	  
		
	  CHECK_EQUAL(true, canHaveChildren);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ElementCanHaveChildrenTest end////////////");
}

TEST(ElementCantHaveChildrenTest)
{
   FE_INFO("/////////  ElementCantHaveChildrenTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_AnonymousTypeTest.xml");
	  bool canHaveChildren = guru->canHaveChildren("test");
		
		
	  CHECK_EQUAL(true, !canHaveChildren);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ElementCantHaveChildrenTest end ////////////");
}

TEST(ValidChildrenForElementTest)
{
   FE_INFO("/////////  ValidChildrenForElementTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  DeclarationSet validChildren = guru->validChildrenForElement("region");

	  FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - Valid children : ")));
	  int testCount, regionCount;
	  for(auto& child : validChildren)
	  {
		 FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - %1%") % child->getName()));
	  }
		
	  testCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "test";}
		 );
		
	  CHECK(testCount>0);

	  regionCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region";}
		 );
		
	  CHECK(regionCount>0);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidChildrenForElementTest end ////////////");
}


// TEST(ValidChildrenForElementOrderTest)
// {
//    FE_INFO("/////////  ValidChildrenForElementOrderTest begin ////////////");
//    try {
// 	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
// 	  DeclarationSet validChildren = guru->validChildrenForElement("region");

// 	  FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - Valid children : ")));
// 	  for(auto& child : validChildren)
// 	  {
// 		 FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - %1%") % child->getName()));
// 	  }

// 	  CHECK(validChildren[0]->getName() == "test");
// 	  CHECK(validChildren[1]->getName() == "region");
//    }
//    catch(std::exception const & ex__){
// 	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
// 	  CHECK(false);
//    }
//    FE_INFO("/////////  ValidChildrenForElementOrderTest end ////////////");
// }


TEST(ValidChildrenAfterElementTest1)
{
   FE_INFO("/////////  ValidChildrenAfterElementTest1 begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("test");
	  existingChildren.push_back("region");
	  DeclarationSet validChildren = guru->validChildrenAfterElement("region", 0, existingChildren);

	  FE_TRACE(__F(("test1::ValidChildrenAfterElementTest1 - Valid children : ")));
	  int testCount, regionCount;
	  for(auto& child : validChildren)
	  {
		 FE_TRACE(__F(("test1::ValidChildrenAfterElementTest1 - %1%") % child->getName()));
	  }

	  testCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "test";}
		 );;
	  regionCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region";}
		 );
	  CHECK(testCount>0);
	  CHECK(regionCount>0);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidChildrenAfterElementTest1 end ////////////");
}

TEST(ValidChildrenBeforeElementTest1)
{
   FE_INFO("/////////  ValidChildrenBeforeElementTest1 begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("test");
	  existingChildren.push_back("region");
	  DeclarationSet validChildren = guru->validChildrenBeforeElement("region", 1, existingChildren);
		
	  FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - Valid children : ")));
	  int testCount, regionCount;
	  for(auto& child : validChildren)
	  {
		 FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - %1%") % child->getName()));
	  }

	  testCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "test";}
		 );;
	  regionCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region";}
		 );
	  CHECK(testCount>0);
	  CHECK(regionCount>0);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidChildrenBeforeElementTest1 end ////////////");
}

TEST(ValidChildrenBeforeElementTest2)
{
   FE_INFO("/////////  ValidChildrenToBeforeElementTest2 begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  GroupDeclaration* regionEl = dynamic_cast<GroupDeclaration*>(guru->getElementDeclaration("region"));
	  CHECK(regionEl->isComplex());
	  CHECK(regionEl->getIndicator() == Indicator::CHOICE);
	  vector<string> existingChildren;
	  existingChildren.push_back("region");
	  DeclarationSet validChildren = guru->validChildrenBeforeElement("region", 0, existingChildren);

	  FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - Valid children : ")));
	  int testCount, regionCount;
	  for(auto& child : validChildren)
	  {
		 FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - %1%") % child->getName()));
	  }

	  testCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "test";}
		 );;
	  regionCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region";}
		 );
	  CHECK(testCount>0);
	  CHECK(regionCount>0);
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidChildrenBeforeElementTest2 end ////////////");
}


TEST(ValidChildrenBeforeElementWithEmptyListParameterTest)
{
   FE_INFO("/////////  ValidChildrenToPrependToElementWithEmptyListParameterTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  vector<string> existingChildren;
	  DeclarationSet validChildren = guru->validChildrenBeforeElement("region", 1, existingChildren);

	  CHECK(validChildren.empty());
   }
   catch(std::exception const & ex__){
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidChildrenToPrependToElementTest end ////////////");
}

TEST(AttributesForElementTest)
{
   FE_INFO("/////////  AttributesForElementTest begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  AttributeDeclarationSet attributes = guru->attributesForElement("region");

	  FE_TRACE("test1::AttributesForElementChildTest - Attributes : ");
	  for(AttributeDeclaration* attr : attributes)
	  {
		 FE_TRACE(__F(("test1:AttributesForElementChildTest - %1%") % attr->getName()));
	  }

	  int testCount = count_if(
		 attributes.begin(),
		 attributes.end(),
		 [](AttributeDeclaration* a){ return a->getName() == "version"; }
		 );
	  CHECK(testCount>0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   
   FE_INFO("/////////  AttributesForElementTest end ////////////");
}

TEST(AttributesForElementChildTest)
{
   FE_INFO("/////////  AttributesForElementTest begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");
	  GroupDeclaration* group = dynamic_cast<GroupDeclaration*>(type);
	  ElementDeclaration* regionEl = group->getChild("region");

	  ComplexElementDeclaration* complexEl = dynamic_cast<ComplexElementDeclaration*>(regionEl);

	  AttributeDeclarationSet attributes = complexEl->getAttributes();

	  FE_TRACE("test1::AttributesForElementChildTest - Attributes : ");
	  for(AttributeDeclaration* attr : attributes)
	  {
		 FE_TRACE(__F(("test1:AttributesForElementChildTest - %1%") % attr->getName()));
	  }

	  int testCount = count_if(
		 attributes.begin(),
		 attributes.end(),
		 [](AttributeDeclaration* a){ return a->getName() == "version"; }
		 );
	  CHECK(testCount>0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   
   FE_INFO("/////////  AttributesForElementTest end ////////////");
}



TEST(MinOccursElementTest)
{
   FE_INFO("/////////  MinOccursElementTest begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");
	  GroupDeclaration* group = dynamic_cast<GroupDeclaration*>(type);
	  ElementDeclaration* regionEl = group->getChild("region");

	  CHECK(regionEl->getMinOccurs() == 0);	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   
   FE_INFO("/////////  MinOccursElementTest end ////////////");
}


TEST(MaxOccursElementTest)
{
   FE_INFO("/////////  MaxOccursElementTest begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");
	  GroupDeclaration* group = dynamic_cast<GroupDeclaration*>(type);
	  ElementDeclaration* regionEl = group->getChild("region");

	  CHECK(regionEl->getMaxOccurs() == 3);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   
   FE_INFO("/////////  MaxOccursElementTest end ////////////");
}

TEST(GetValidChildrenForExistingListTest)
{
   FE_INFO("/////////  GetValidChildrenForExistingListTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* type = guru->getElementDeclaration("region");
	  vector<string> existingChildren;
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("region");
	  
	  DeclarationSet validChildren = guru->validChildrenForElement("region", existingChildren);

	  FE_TRACE("test1::GetValidChildrenForExistingListTest - Valid children : ");
	  for(ElementDeclaration* s : validChildren)
	  {
		 FE_TRACE(__F(("test1::GetValidChildrenForExistingListTest - %1%") % s->getName()));
	  }

	  int regionCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region";}
		 );
	  
	  CHECK(regionCount > 0);
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  GetValidChildrenForExistingListTest begin ////////////");
}

TEST(GetValidPositionsForElementInExistingListTest)
{
   FE_INFO("/////////  GetValidPositionsForElementInExistingListTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementSequenceTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("test");
	  existingChildren.push_back("test2");
	  
	  vector<int> validPositions = guru->validPositionsForElement("region", "test2", existingChildren);

	  FE_TRACE("test::GetValidPositionsForElementInExistingListTest - Valid positions: ");
	  for(int i : validPositions)
	  {
		 FE_TRACE(__F(("test::GetValidPositionsForElementInExistingListTest - %1%") % i));
	  }

	  int lastPositionCount = std::count(
		 validPositions.begin(),
		 validPositions.end(),
		 5
		 );
	  
	  CHECK(lastPositionCount > 0);
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  GetValidPositionsForElementInExistingListTest begin ////////////");
}

TEST(GetRootNodesTest)
{
   FE_INFO("/////////  GetRootNodesTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_DoubleRootTest.xml");
	  DeclarationSet rootNodes = guru->rootNodes();

	  FE_TRACE("test1::GetRootNodesTest  - Root nodes: ");
	  for(ElementDeclaration* e : rootNodes)
	  {
		 FE_TRACE(__F(("test1::GetRootNodesTest  - %1%") %  e->getName()));
	  }
	  
	  int regionCount = std::count_if(
		 rootNodes.begin(),
		 rootNodes.end(),
		 [](ElementDeclaration* e){return e->getName() == "region";}
		 );
	  int compCount = std::count_if(
		 rootNodes.begin(),
		 rootNodes.end(),
		 [](ElementDeclaration* e){ return e->getName() == "component";}
		 );
	  CHECK(regionCount > 0);
	  CHECK(compCount > 0);
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  GetRootNodesTest begin ////////////");
}

TEST(GetIndicatorMinOccursTest)
{
   FE_INFO("/////////  GetIndicatorMinOccursTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* el = guru->getElementDeclaration("region");
	  GroupDeclaration* group = dynamic_cast<GroupDeclaration*>(el);

	  CHECK(group->getIndicatorMinOccurs() == 1);
	  

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  GetIndicatorMinOccursTest begin ////////////");
}

TEST(GetIndicatorMaxOccursTest)
{
   FE_INFO("/////////  GetIndicatorMinOccursTest begin ////////////");
   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_ElementTypeTest.xml");
	  ElementDeclaration* el = guru->getElementDeclaration("region");
	  GroupDeclaration* group = dynamic_cast<GroupDeclaration*>(el);

	  CHECK(group->getIndicatorMaxOccurs() == INT_MAX);
	  

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  GetIndicatorMinOccursTest begin ////////////");
}

TEST(SequenceInsertionTest1)
{
   FE_INFO("/////////  SequenceInsertionTest1 begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_DoubleRootTest.xml");
	  ElementDeclaration* el = guru->getElementDeclaration("region");

	  vector<string> existingChildren;
	  existingChildren.push_back("test");
	  
	  vector<int> positions = guru->validPositionsForElement("region", "a", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest - Checking positions for a");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest - position before index %1%") % i));
	  }
	  int check = count(positions.begin(), positions.end(), 1);
	  CHECK(check > 0);

	  existingChildren.push_back("a");
	  positions = guru->validPositionsForElement("region", "b", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest - Checking positions for b");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest - position before index %1%") % i));
	  }
	  check = count(positions.begin(), positions.end(), 2);
	  CHECK(check > 0);

	  existingChildren.push_back("b");
	  positions = guru->validPositionsForElement("region", "c", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest - Checking positions for c");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest - position before index %1%") % i));
	  }
	  check = count(positions.begin(), positions.end(), 3);

	  CHECK(check > 0);

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  SequenceInsertionTest1 end ////////////");
}

TEST(SequenceInsertionTest2)
{
   FE_INFO("/////////  SequenceInsertionTest2 begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_SequenceInsertTest.xml");
	  ElementDeclaration* el = guru->getElementDeclaration("region");

	  vector<string> existingChildren;
	  existingChildren.push_back("c");

	  DeclarationSet validChildren = guru->validChildrenForElement("region", existingChildren);
	  int childCheck = count_if(validChildren.begin(), validChildren.end(),
							 [](ElementDeclaration* e){ return e->getName() == "c";});
	  CHECK(childCheck == 0);
	  vector<int> positions = guru->validPositionsForElement("region", "a", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest2 - Checking positions for a");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest2 - position before index %1%") % i));
	  }
	  int check = count(positions.begin(), positions.end(), 0);
	  CHECK(check > 0);

	  existingChildren.insert( existingChildren.begin(), "a");
	  validChildren = guru->validChildrenForElement("region", existingChildren);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "c";});
	  CHECK(childCheck == 0);
	  positions = guru->validPositionsForElement("region", "b", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest2 - Checking positions for b");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest2 - position before index %1%") % i));
	  }
	  check = count(positions.begin(), positions.end(), 1);
	  CHECK(check > 0);

	  existingChildren.insert((++existingChildren.begin()), "b");
	  validChildren = guru->validChildrenForElement("region", existingChildren);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "c";});
	  CHECK(childCheck == 0);
	  positions = guru->validPositionsForElement("region", "b", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest2 - Checking positions for b");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest2 - position before index %1%") % i));
	  }
	  check = count(positions.begin(), positions.end(), 1);
	  CHECK(check > 0);
	  check = count(positions.begin(), positions.end(), 2);
	  CHECK(check > 0);

	  existingChildren.insert((++existingChildren.begin()), "b");
	  validChildren = guru->validChildrenForElement("region", existingChildren);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "c";});
	  CHECK(childCheck == 0);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "b";});
	  CHECK(childCheck == 0);
	  

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  SequenceInsertionTest2 end ////////////");
}


TEST(SequenceInsertionTest3)
{
   FE_INFO("/////////  SequenceInsertionTest3 begin ////////////");

   try {
	  XSDGuru* guru = new XSDGuru("/tmp/test1_SequenceInsertTest.xml");
	  ElementDeclaration* el = guru->getElementDeclaration("region");

	  vector<string> existingChildren;
	  existingChildren.push_back("a");
	 
	  vector<int> positions = guru->validPositionsForElement("region", "a", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest3 - Checking positions for a");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest3 - position before index %1%") % i));
	  }
	  int check = count(positions.begin(), positions.end(), 0);
	  CHECK(check > 0);
	  check = count(positions.begin(), positions.end(), 1);
	  CHECK(check > 0);

	  existingChildren.push_back("b");
	  existingChildren.push_back("b");
	  DeclarationSet validChildren = guru->validChildrenForElement("region", existingChildren);
	  int childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "b";});
	  CHECK(childCheck == 0);

	  CHECK(check > 0);

	  positions = guru->validPositionsForElement("region", "c", existingChildren);
	  FE_INFO("test1::SequenceInsertionTest2 - Checking positions for c");
	  for(int i : positions)
	  {
		 FE_INFO(__F(("test1::SequenceInsertionTest2 - position before index %1%") % i));
	  }
	  check = count(positions.begin(), positions.end(), 3);
	  CHECK(check > 0);

	  existingChildren.push_back("c");
	  validChildren = guru->validChildrenForElement("region", existingChildren);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "c";});
	  CHECK(childCheck == 0);
	  childCheck = count_if(validChildren.begin(), validChildren.end(),
						 [](ElementDeclaration* e){ return e->getName() == "b";});
	  CHECK(childCheck == 0);
	  

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("/////////  SequenceInsertionTest3 end ////////////");
}

TEST(ValidRemoveTest1)
{
   FE_INFO("/////////  ValidRemoveTest1 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SequenceRemoveTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("aaa");
	  existingChildren.push_back("bbb");
	  existingChildren.push_back("ccc");
	  existingChildren.push_back("aaa");
	  existingChildren.push_back("bbb");
	  existingChildren.push_back("ccc");
	  vector<int> invalidIndex = guru.invalidIndexesAfterRemove("region", 3, existingChildren);
	  
	  FE_INFO(__F(("test1::ValidRemoveTest1 - Checking positions for removal (count: %1%)") % invalidIndex.size()));
	  for(int i : invalidIndex)
	  {
		 FE_INFO(__F(("test1::ValidRemoveTest1 - %1%") % i));
	  }
	  
	  int testCount = count(invalidIndex.begin(), invalidIndex.end(), 3);
	  CHECK(testCount > 0);
	  testCount = count(invalidIndex.begin(), invalidIndex.end(), 4);
	  CHECK(testCount > 0);
	  testCount = count(invalidIndex.begin(), invalidIndex.end(), 5);
	  CHECK(testCount > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidRemoveTest1 end ////////////");
}

TEST(ValidRemoveTest2)
{
   FE_INFO("/////////  ValidRemoveTest2 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SequenceRemoveTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("aaa");
	  existingChildren.push_back("bbb");
	  existingChildren.push_back("ccc");
	  existingChildren.push_back("aaa");
	  existingChildren.push_back("bbb");
	  existingChildren.push_back("ccc");
	  vector<int> invalidIndex = guru.invalidIndexesAfterRemove("region", 4, existingChildren);
	  
	  FE_INFO(__F(("test1::ValidRemoveTest2 - Checking positions for removal (count: %1%)") % invalidIndex.size()));
	  for(int i : invalidIndex)
	  {
		 FE_INFO(__F(("test1::ValidRemoveTest2 - %1%") % i));
	  }
	  
	  int testCount = count(invalidIndex.begin(), invalidIndex.end(), 3);
	  CHECK(testCount > 0);
	  testCount = count(invalidIndex.begin(), invalidIndex.end(), 4);
	  CHECK(testCount > 0);
	  testCount = count(invalidIndex.begin(), invalidIndex.end(), 5);
	  CHECK(testCount > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidRemoveTest2 end ////////////");
}

TEST(ValidRemoveTest3)
{
   FE_INFO("/////////  ValidRemoveTest3 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SequenceRemoveTest.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("aaa");
	  existingChildren.push_back("bbb");
	  existingChildren.push_back("ccc");
	  
	  
	  vector<int> invalidIndex = guru.invalidIndexesAfterRemove("region", 0, existingChildren);
	  
	  FE_INFO(__F(("test1::ValidRemoveTest3 - Checking positions for removal (count: %1%)") % invalidIndex.size()));
	  for(int i : invalidIndex)
	  {
		 FE_INFO(__F(("test1::ValidRemoveTest3 - %1%") % i));
	  }
	  
	  int testCount = count(invalidIndex.begin(), invalidIndex.end(), 0);
	  CHECK(testCount > 0);

	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidRemoveTest3 end ////////////");
}

TEST(ValidRemoveTest4)
{
   FE_INFO("/////////  ValidRemoveTest4 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SequenceRemoveTest2.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("aaa");
	  
	  vector<int> invalidIndex = guru.invalidIndexesAfterRemove("region", 0, existingChildren);
	  
	  FE_INFO(__F(("test1::ValidRemoveTest4 - Checking positions for removal (count: %1%)") % invalidIndex.size()));
	  for(int i : invalidIndex)
	  {
		 FE_INFO(__F(("test1::ValidRemoveTest4 - %1%") % i));
	  }
	  
	  int testCount = count(invalidIndex.begin(), invalidIndex.end(), 0);
	  CHECK(testCount > 0);

	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  ValidRemoveTest4 end ////////////");
}

TEST(SimpleContentTest)
{
   FE_INFO("/////////  SimpleContentTest begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SimpleContentTest.xml");
	  ComplexElementDeclaration* c = dynamic_cast<ComplexElementDeclaration*>(guru.getElementDeclaration("scElement"));

	  CHECK(c->getContentType() == ContentType::SIMPLE_CONTENT);

	  SimpleContentDeclaration* scd = dynamic_cast<SimpleContentDeclaration*>(c);
	  AttributeDeclarationSet attrSet = scd->getAttributes(); 
	  for(AttributeDeclaration* attr : attrSet)
	  {
		 FE_TRACE(__F(("Found attribute %1%") % attr->getName()));
	  }

	  int check = count_if(
		 attrSet.begin(),
		 attrSet.end(),
		 [](AttributeDeclaration* a){ return a->getName() == "testAttr"; }
		 );

	  CHECK(check > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   FE_INFO("/////////  SimpleContentTest end ////////////");
}

TEST(SimpleContentTest2)
{
   FE_INFO("/////////  SimpleContentTest2 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SimpleContentTest.xml");
	  ComplexElementDeclaration* c = dynamic_cast<ComplexElementDeclaration*>(guru.getElementDeclaration("resElement"));

	  CHECK(c->getContentType() == ContentType::SIMPLE_CONTENT);

	  SimpleContentDeclaration* scd = dynamic_cast<SimpleContentDeclaration*>(c);
	  IntDeclaration* intDecl = dynamic_cast<IntDeclaration*>(scd->getTypeDeclaration());

	  CHECK(intDecl->getTotalDigits() == 5);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   FE_INFO("/////////  SimpleContentTest2 end ////////////");
}

TEST(SimpleContentTest3)
{
   FE_INFO("/////////  SimpleContentTest3 begin ////////////");
   try {
	  XSDGuru guru("/tmp/test1_SimpleContentTest.xml");
	  ComplexElementDeclaration* c = dynamic_cast<ComplexElementDeclaration*>(guru.getElementDeclaration("nestresElement"));

	  CHECK(c->getContentType() == ContentType::SIMPLE_CONTENT);

	  SimpleContentDeclaration* scd = dynamic_cast<SimpleContentDeclaration*>(c);
	  StringDeclaration* stringDecl = dynamic_cast<StringDeclaration*>(scd->getTypeDeclaration());

	  CHECK(stringDecl->getMinLength() == 5);
	  CHECK(stringDecl->getMaxLength() == 10);	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   FE_INFO("/////////  SimpleContentTest3 end ////////////");
}

TEST(FullXSDTest)
{
   FE_INFO("/////////  FullXSDTest start ////////////");
   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xml");
	  ElementDeclaration* el = guru.getElementDeclaration("component");
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   FE_INFO("/////////  FullXSDTest end ////////////");
}

TEST(AttributeTypeTest)
{
   FE_INFO("/////////  AttributeTypeTest end ////////////");
   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xml");
	  ElementDeclaration* el = guru.getElementDeclaration("value");
	  GroupDeclaration* valueEl = dynamic_cast<GroupDeclaration*>(el);

	  //GroupDeclaration* valueEl = dynamic_cast<GroupDeclaration*>(vEl);

	  AttributeDeclarationSet attributes = valueEl->getAttributes();

	  auto it = find_if(
	  	 attributes.begin(),
	  	 attributes.end(),
	  	 [](AttributeDeclaration * a){ return a->getName() == "size";}
	  	 );

	  TypeDeclaration* typeDecl = (*it)->getTypeDeclaration();
	  IntDeclaration* intDecl = dynamic_cast<IntDeclaration*>(typeDecl);

	  CHECK(intDecl->getMax() == 32);
	  CHECK(intDecl->getMin() == 0);
	  
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }
   FE_INFO("/////////  AttributeTypeTest end ////////////");
}

TEST(ComplexContentTest)
{
   FE_INFO("/////////  ComplexContentTest begin ////////////");

   try {
	  XSDGuru guru("/tmp/test1_ComplexContentTest.xml");
	  ComplexElementDeclaration* c = dynamic_cast<ComplexElementDeclaration*>(guru.getElementDeclaration("cElement"));

	  CHECK(c->getContentType() == ContentType::GROUP);

	  ComplexContentDeclaration* ccd = dynamic_cast<ComplexContentDeclaration*>(c);

	  DeclarationSet children = ccd->getChildren();

	  function<void(DeclarationSet)> printChildren;
	  printChildren = [&printChildren](DeclarationSet children) -> void {
		 for(ElementDeclaration* s : children)
		 {
			FE_TRACE(__F(("test1::ComplexContentTest - %1%") % s->getName()));
			if(s->isComplex())
			{
			   ComplexElementDeclaration* ced = dynamic_cast<ComplexElementDeclaration*>(s);
			   if(ced->getContentType() == ContentType::GROUP)
			   {
				  GroupDeclaration* gd = dynamic_cast<GroupDeclaration*>(ced);
				  printChildren(gd->getChildren());
			   }
			}
		 }
	  };
	  printChildren(children);

	  ElementDeclaration* testEl = ccd->getChild("testEL");
	  if(!testEl)
	  {
		 FE_TRACE("test1::ComplexContentTest - testEL is null!");
	  } else {
		 FE_TRACE(__F(("TEST: %1%") % testEl->getName()));
	  }
	  CHECK(testEl->getTypeDeclaration()->getType() == ElementType::INT);

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 
   
   FE_INFO("/////////  ComplexContentTest end ////////////");
}

TEST(ComplexContentTest2)
{
   FE_INFO("/////////  ComplexContentTest2 begin ////////////");

   try {
	  XSDGuru guru("/tmp/test1_ComplexContentTest.xml");

	  DeclarationSet validChildren = guru.validChildrenForElement("cElement");

	  FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - Valid children : ")));

	  for(auto& child : validChildren)
	  {
		 FE_TRACE(__F(("test1::ValidChildrenBeforeElementTest2 - %1%") % child->getName()));
	  }
		
	  int testCount = std::count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "default";}
		 );
		
	  CHECK(testCount>0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 
   
   FE_INFO("/////////  ComplexContentTest2 end ////////////");
}

TEST(FullXSDTest2)
{
   FE_INFO("/////////  FullXSDTest2 end ////////////");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("value");

	  DeclarationSet validChildren = guru.validChildrenForElement("region", existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest2 - valid child %1%") % el->getName()));
	  }

	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "description"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "value"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "block"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "padding"; }
		 );

	  CHECK(check > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   
   FE_INFO("/////////  FullXSDTest2 end ////////////");
}

TEST(FullXSDTest3)
{
   FE_INFO("/////////  FullXSDTest3 end ////////////");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xml");
	  vector<string> existingChildren;
	  existingChildren.push_back("description");
	  existingChildren.push_back("label");
	  existingChildren.push_back("region");

	  DeclarationSet validChildren = guru.validChildrenForElement("component", existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest2 - valid child %1%") % el->getName()));
	  }

	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "region"; }
		 );

	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   
   FE_INFO("/////////  FullXSDTest3 end ////////////");
}

TEST(FullXSDTest4)
{
   FE_INFO("/////////  FullXSDTest4 end ////////////");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  vector<string> existingChildren;

	  DeclarationSet validChildren = guru.validChildrenForElement("component", existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest4 - valid child %1%") % el->getName()));
	  }

	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "status-area"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "control-area"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "ram"; }
		 );

	  CHECK(check > 0);

	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 

   
   FE_INFO("/////////  FullXSDTest4 end ////////////");
}

TEST(FullXSDTest5)
{
   FE_INFO("/////////  FullXSDTest5 end ////////////");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  vector<string> existingChildren;

	  DeclarationSet validChildren = guru.validChildrenForElement("status-area", existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest5 - valid child %1%") % el->getName()));
	  }
	  
	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "status-area"; }
		 );

	  CHECK(check > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 
   
   FE_INFO("/////////  FullXSDTest5 end ////////////");
}

TEST(FullXSDTest6)
{
   FE_INFO("/////////  FullXSDTest6 end ////////////");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  vector<string> existingChildren;
	  existingChildren.push_back("status-area");
	  DeclarationSet validChildren = guru.validChildrenForElement("status-area", existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest6 - valid child %1%") % el->getName()));
	  }

	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "status-area"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "register"; }
		 );

	  CHECK(check > 0);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 
   
   FE_INFO("/////////  FullXSDTest6 end ////////////");
}

TEST(FullXSDTest7)
{
   FE_INFO("------------------------- FullXSDTest7 start -------------------------");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  vector<string> existingChildren;
	  existingChildren.push_back("component");
	  existingChildren.push_back("ram");
	  DeclarationSet validChildren = guru.validChildrenBeforeElement("component", 1, existingChildren);

	  for(ElementDeclaration* el : validChildren)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest7 - valid child before %1%") % el->getName()));
	  }

	  CHECK(validChildren.size() > 0);
/*
	  int check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "status-area"; }
		 );

	  CHECK(check > 0);

	  check = count_if(
		 validChildren.begin(),
		 validChildren.end(),
		 [](ElementDeclaration* e){ return e->getName() == "register"; }
		 );

	  CHECK(check > 0);*/

   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- FullXSDTest7 end -------------------------");
}

TEST(FullXSDTest8)
{
   FE_INFO("------------------------- FullXSDTest8 start -------------------------");

   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  vector<string> existingChildren;
	  std::string parent_name__ = "register";
	  FE_TRACE(__F(("test1::FullXSDTest8 - all valid child for %1%") % parent_name__));
	  DeclarationSet validChildren1 = guru.validChildrenForElement(parent_name__, existingChildren);
	  for(ElementDeclaration* el : validChildren1)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest8 - valid child: %1%") % el->getName()));
	  }
	  CHECK(validChildren1.size() > 0);

	  int check = count_if(
		 validChildren1.begin(),
		 validChildren1.end(),
		 [](ElementDeclaration* e){ return e->getName() == "description"; }
		 );

	  CHECK(check == 1);

	  check = count_if(
		 validChildren1.begin(),
		 validChildren1.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check == 1);

	  FE_TRACE(__F(("test1::FullXSDTest8 - valid child for %1% with description as only child") % parent_name__));
	  existingChildren.push_back("description");
	  DeclarationSet validChildren2 = guru.validChildrenForElement("register", existingChildren);

	  for(ElementDeclaration* el : validChildren2)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest8 - valid child: %1%") % el->getName()));
	  }

	  CHECK(validChildren2.size() > 0);

	  check = count_if(
		 validChildren2.begin(),
		 validChildren2.end(),
		 [](ElementDeclaration* e){ return e->getName() == "description"; }
		 );

	  CHECK(check == 0);

	  check = count_if(
		 validChildren2.begin(),
		 validChildren2.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check == 1);

	  FE_TRACE(__F(("test1::FullXSDTest8 - valid child for %1% with description and label as children") % parent_name__));
	  existingChildren.push_back("label");
	  DeclarationSet validChildren3 = guru.validChildrenForElement("register", existingChildren);

	  for(ElementDeclaration* el : validChildren3)
	  {
		 FE_TRACE(__F(("test1::FullXSDTest8 - valid child: %1%") % el->getName()));
	  }

	  CHECK(validChildren3.size() > 0);

	  check = count_if(
		 validChildren3.begin(),
		 validChildren3.end(),
		 [](ElementDeclaration* e){ return e->getName() == "description"; }
		 );

	  CHECK(check == 0);

	  check = count_if(
		 validChildren3.begin(),
		 validChildren3.end(),
		 [](ElementDeclaration* e){ return e->getName() == "label"; }
		 );

	  CHECK(check == 1);


   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- FullXSDTest8 end -------------------------");
}

TEST(ValidationTest)
{
   FE_INFO("------------------------- ValidationTest end -------------------------");
   try {
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");

	  ifstream fin("/tmp/test1_TestXML.xml");

	  if ( fin.fail())
		 CHECK(false);
	  
	  fin.seekg(0, ios::end);
	  size_t length = fin.tellg();
	  fin.seekg(0, ios::beg);
	  
	  char* buffer = new char[length+1];
	  fin.read(buffer, length);
	  buffer[length] = '\0';
	  
	  fin.close();

	  bool result = guru.validateData(buffer);

	  delete[] buffer;

	  CHECK(result);
	  
   } catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   } 
   
   FE_INFO("------------------------- ValidationTest end -------------------------");
   
}

TEST(ValidationTestXMLParseFailure)
{
   FE_INFO("------------------------- ValidationTestXMLParseFailure start -------------------------");
   try
   {
      FE_INFO("Creating guru");
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  FE_INFO("Creating guru done");
	  ifstream fin("/tmp/test_validation_xml_parse_failure.xml");

	  if ( fin.fail())
		 CHECK(false);

	  fin.seekg(0, ios::end);
	  size_t length = fin.tellg();
	  fin.seekg(0, ios::beg);

	  char* buffer = new char[length+1];
	  fin.read(buffer, length);
	  buffer[length] = '\0';

	  fin.close();

	  FE_INFO("Validating start");
	  bool result = guru.validateData(buffer);
	  FE_INFO("Validating end");

	  delete[] buffer;

	  CHECK(!result);
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- ValidationTestXMLParseFailure end -------------------------");

}

TEST(ValidationTestXMLInvalidTag)
{
   FE_INFO("------------------------- ValidationTestXMLInvalidTag start -------------------------");
   try
   {
      FE_INFO("Creating guru");
	  XSDGuru guru("/tmp/test1_FullXSD.xsd");
	  FE_INFO("Creating guru done");

	  ifstream fin("/tmp/test_validation_xml_invalid_tag.xml");

	  if ( fin.fail())
		 CHECK(false);

	  fin.seekg(0, ios::end);
	  size_t length = fin.tellg();
	  fin.seekg(0, ios::beg);

	  char* buffer = new char[length+1];
	  fin.read(buffer, length);
	  buffer[length] = '\0';

	  fin.close();

	  FE_INFO("Validating start");
	  bool result = guru.validateData(buffer);
	  FE_INFO("Validating end");

	  delete[] buffer;

	  CHECK(!result);

   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- ValidationTestXMLInvalidTag end -------------------------");

}

TEST(RegexGuruTest1)
{
   FE_INFO("------------------------- RegexGuruTest1 start -------------------------");
   try
   {
     FE_INFO("Creating guru");
     RegexGuru guru("((aa)(bb)+(cc)*(dd)?)");
     FE_INFO("Creating guru done");
     vector< vector<string> > validPositions = guru.getValidChildrenPositions("aabb");

     for(int i=0; i< validPositions.size(); i++){
       FE_INFO(__F(("Valid elements at position %1% :") % i));
       for(string s : validPositions[i]){
     	 FE_INFO(__F(("---name : %1%") % s));
       }
     }
     // for(auto it : validPositions){
     //   FE_INFO(__F(("Valid positions for %1% :") % it.first));
     //   for(int i : it.second){
     // 	 FE_INFO(__F(("---position : %1%") % i));
     //   }
     // }
     
     
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- RegexGuruTest1 end -------------------------");

}

TEST(RegexGuruTest2)
{
   FE_INFO("------------------------- RegexGuruTest2 start -------------------------");
   try
   {
     FE_INFO("Creating guru");
     RegexGuru guru("((aa)(bb)+(cc)*(dd)?)");
     FE_INFO("Creating guru done");
     vector< vector<string> > validPositions = guru.getValidChildrenPositions("aabbdd");

     for(int i=0; i< validPositions.size(); i++){
       FE_INFO(__F(("Valid elements at position %1% :") % i));
       for(string s : validPositions[i]){
     	 FE_INFO(__F(("---name : %1%") % s));
       }
     }
     // for(auto& it : validPositions){
     //   FE_INFO(__F(("Valid positions for %1% :") % it.first));
     //   for(int i : it.second){
     // 	 FE_INFO(__F(("---position : %1%") % i));
     //   }
     // }    
     
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- RegexGuruTest2 end -------------------------");

}

TEST(RegexGuruTest3)
{
   FE_INFO("------------------------- RegexGuruTest3 start -------------------------");
   try
   {
     FE_INFO("Creating guru");
     RegexGuru guru("((aa)(bb)+(cc)*(dd)?)");
     FE_INFO("Creating guru done");
     vector< vector<string> > validPositions = guru.getValidChildrenPositions("bbbbccdd");


     for(int i=0; i< validPositions.size(); i++){
       FE_INFO(__F(("Valid elements at position %1% :") % i));
       for(string s : validPositions[i]){
     	 FE_INFO(__F(("---name : %1%") % s));
       }
     }
     // for(auto& it : validPositions){
     //   FE_INFO(__F(("Valid positions for %1% :") % it.first));
     //   for(int i : it.second){
     // 	 FE_INFO(__F(("---position : %1%") % i));
     //   }
     // }    
     
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- RegexGuruTest3 end -------------------------");

}

TEST(BoostLibraryRegexTest)
{
   FE_INFO("------------------------- BoostLibraryRegexTest start -------------------------");
   try
   {
	   //simple regex
	   boost::smatch simpleWhat;
	   boost::regex simpleRegex = boost::regex("(x+x+)+y");
	   string matchString = "xxxxxy";

	   
	   if(boost::regex_match(matchString, simpleWhat, simpleRegex)){
		   FE_INFO("Regex matched simple case successfully");
	   } else{
		   FE_INFO("Regex matched simple case unsuccessfully");
	   }

	   
	   boost::smatch what;
	   boost::regex regexObj = boost::regex("((description){0,1}(label)*(((ram)+|(status-area)+|(control-area)+|(component)+)*)*){0,1}");

	   string tempString = "componentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(boost::regex_match(tempString, what, regexObj, boost::match_not_null)){
		   FE_INFO("Regex matched short string successfully");
 	   } else
	   {
		   FE_INFO("Regex did not match short string");
	   }

	   //string which should match
	   tempString = "control-areastatus-areacontrol-area";
	   if(boost::regex_match(tempString, what, regexObj, boost::match_not_null)){
		   FE_INFO("Regex matched valid case successfully");
 	   } else {
		   FE_ERROR("Regex did not match valid example string!");
		   CHECK(false);
	   }

	   //long string will crash
	   tempString = "componentcomponentcomponentcomponentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(boost::regex_match(tempString, what, regexObj, boost::match_not_null)){
		   FE_INFO("Regex matched long string successfully");
 	   } else
	   {
		   FE_INFO("Regex did not match long string");
	   }
	   //if regex_match doesnt raise exception, test succeeds
	   CHECK(true);
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- BoostLibraryRegexTest end -------------------------");

}

TEST(StdLibraryRegexTest)
{
   FE_INFO("------------------------- StdLibraryRegexTest start -------------------------");
   try
   {
	   //simple regex
	   std::smatch simpleWhat;
	   std::regex simpleRegex = std::regex("(x+x+)+y");
	   string matchString = "xxxxxy";
	   
	   if(std::regex_match(matchString, simpleWhat, simpleRegex, std::regex_constants::match_not_null)){
		   FE_INFO("Regex matched simple case successfully");
	   } else{
		   FE_INFO("Regex matched simple case unsuccessfully");
		   CHECK(false);
	   }
	   
	   std::smatch what;
	   std::regex regexObj = std::regex("((description){0,1}(label)*(((ram)+|(status-area)+|(control-area)+|(component)+)*)*){0,1}", std::regex_constants::ECMAScript);

	   string tempString = "componentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(std::regex_match(tempString, what, regexObj, std::regex_constants::match_not_null)){
		   FE_INFO("Regex matched short string successfully");
 	   }

	   //string which should match
	   tempString = "control-areastatus-areacontrol-area";
	   if(std::regex_match(tempString, what, regexObj, std::regex_constants::match_not_null)){
		   FE_INFO("Regex matched valid case successfully");
 	   } else {
		   FE_ERROR("Regex did not match valid example string!");
		   CHECK(false);
	   }
	   
	   tempString = "componentcomponentcomponentcomponentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(std::regex_match(tempString, what, regexObj, std::regex_constants::match_not_null)){
		   FE_INFO("Regex matched successfully");
 	   }
	   //if regex_match doesnt raise exception, test succeeds
	   CHECK(true);
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- StdLibraryRegexTest end -------------------------");

}

TEST(RE2LibraryRegexTest)
{
   FE_INFO("------------------------- RE2LibraryRegexTest start -------------------------");
   try
   {
	   //simple regex
	   RE2 simpleRegex("(x+x+)+y");
	   string matchString = "xxxxxy";
	   
	   if(RE2::FullMatch(matchString, simpleRegex)){
		   FE_INFO("Regex matched simple case successfully");
	   } else{
		   FE_INFO("Regex matched simple case unsuccessfully");
		   CHECK(false);
	   }
	   
	   RE2 regexObj("((description){0,1}(label)*(((ram)+|(status-area)+|(control-area)+|(component)+)*)*){0,1}");

	   string tempString = "componentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(RE2::FullMatch(tempString, regexObj)){
		   FE_INFO("Regex matched short string successfully");
 	   }

	   //string which should match
	   tempString = "control-areastatus-areacontrol-area";
	   if(RE2::FullMatch(tempString, regexObj)){
		   FE_INFO("Regex matched valid case successfully");
 	   } else {
		   FE_ERROR("Regex did not match valid example string!");
		   CHECK(false);
	   }
	   
	   tempString = "componentcomponentcomponentcomponentcontrol-areacontrol-areacomponentstatus-areacomponentcontrol-areacomponentdescription";
	   if(RE2::FullMatch(tempString, regexObj)){
		   FE_INFO("Regex matched successfully");
 	   }
	   //if regex_match doesnt raise exception, test succeeds
	   CHECK(true);
   }
   catch(std::exception const & ex__)
   {
	  FE_ERROR(__F(("Exception caught: %1%") % ex__.what()));
	  CHECK(false);
   }

   FE_INFO("------------------------- RE2LibraryRegexTest end -------------------------");

}


int main( int argc, char** argv )
{
   if( argc > 1 )
   {
      //if first arg is "suite", we search for suite names instead of test names
	  const bool bSuite = strcmp( "suite", argv[ 1 ] ) == 0;

      //walk list of all tests, add those with a name that
      //matches one of the arguments  to a new TestList
	  const TestList& allTests( Test::GetTestList() );
	  TestList selectedTests;
	  Test* p = allTests.GetHead();
	  while( p )
	  {
		 for( int i = 1 ; i < argc ; ++i )
			if( strcmp( bSuite ? p->m_details.suiteName
						: p->m_details.testName, argv[ i ] ) == 0 )
          selectedTests.Add( p );
		 p = p->next;
	  }
	  
      //run selected test(s) only
	  TestReporterStdout reporter;
	  TestRunner runner( reporter );
    return runner.RunTestsIf( selectedTests, 0, True(), 0 );
   }
   else
   {
	  return RunAllTests();
   }
}
