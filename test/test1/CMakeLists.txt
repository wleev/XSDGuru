
file(GLOB sources *.cpp)

set(CMAKE_CXX_FLAGS "-std=gnu++0x")

find_package(LibXmlPP REQUIRED)
find_package(UnitTest++ REQUIRED)

include_directories (
   ${LIBXMLPP_INCLUDE_DIR}
   ${UNITTEST++_INCLUDE_DIR}
)

add_definitions(${LIBXMLPP_DEFINITIONS})

add_executable(${PROJECT_NAME}.test1 ${sources})

target_link_libraries(${PROJECT_NAME}.test1
   ${UNITTEST++_LIBRARY}
   ${PROJECT_NAME}
)

set(test_files
  test1_ElementTypeTest.xml 
  test1_AnonymousTypeTest.xml 
  test1_ElementSequenceTest.xml 
  test1_DoubleRootTest.xml 
  test1_SequenceInsertTest.xml 
  test1_SequenceRemoveTest.xml
  test1_SequenceRemoveTest2.xml
  test1_SimpleContentTest.xml
  test1_FullXSD.xml
  test1_FullXSD.xsd
  test1_ComplexContentTest.xml
  test1_TestXML.xml
  test_validation_xml_parse_failure.xml
  test_validation_xml_invalid_tag.xml
)

install(FILES ${test_files} DESTINATION /tmp)

add_test(NAME ${PROJECT_NAME}.test1 COMMAND ${PROJECT_NAME}.test1)
