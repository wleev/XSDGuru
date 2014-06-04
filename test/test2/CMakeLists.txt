
file(GLOB sources *.cpp)

set(CMAKE_CXX_FLAGS "-std=gnu++0x")

find_package(LibXmlPP REQUIRED)
find_package(UnitTest++ REQUIRED)

include_directories (
   ${LIBXMLPP_INCLUDE_DIR}
   ${UNITTEST++_INCLUDE_DIR}
)

add_definitions(${LIBXMLPP_DEFINITIONS})

add_executable(${PROJECT_NAME}.test2 ${sources})

target_link_libraries(${PROJECT_NAME}.test2
   ${UNITTEST++_LIBRARY}
   ${PROJECT_NAME}
)
   
set(test_files
   test2_SimpleTypeTest.xml
)

install(FILES ${test_files} DESTINATION /tmp)

add_test(NAME ${PROJECT_NAME}.test2 COMMAND ${PROJECT_NAME}.test2)
