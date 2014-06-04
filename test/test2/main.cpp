#include <unittest++/UnitTest++.h>

#include <iostream>

#include <tools/ssl/xsd/xsd_guru.hpp>


using namespace std;

TEST(SimpleTypeStringTest)
{
  try {
	  xsd_guru* guru = new xsd_guru("/tmp/test2_SimpleTypeTest.xml");
	  xsd_type* type = guru->get_element_type("test");
	  
	  CHECK(type->content_type_ == content_type::SIMPLE);
  }
  catch(std::exception const & ex__){
	  cerr << "Exception caught: " << ex__.what() << endl;
	  CHECK(false);
  }
}

TEST(SimpleTypeIntegerTest)
{
  try {
	  xsd_guru* guru = new xsd_guru("/tmp/test2_SimpleTypeTest.xml");
	  xsd_type* type = guru->get_element_type("test2");
	  
	  CHECK(type->content_type_ == content_type::SIMPLE);
  }
  catch(std::exception const & ex__){
	  cerr << "Exception caught: " << ex__.what() << endl;
	  CHECK(false);
  }
}

TEST(SimpleTypeExtendedIntegerTest)
{
  try {
	  xsd_guru* guru = new xsd_guru("/tmp/test2_SimpleTypeTest.xml");
	  xsd_type* type = guru->get_element_type("test3");
	  
	  CHECK(type->content_type_ == content_type::SIMPLE);
  }
  catch(std::exception const & ex__){
	  cerr << "Exception caught: " << ex__.what() << endl;
	  CHECK(false);
  }
}

int main() {
	return UnitTest::RunAllTests();
}
