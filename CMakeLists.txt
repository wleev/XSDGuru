project(xsdguru)

file(GLOB sources *.cpp model/*.cpp)

file(GLOB headers *.h *.hpp model/*.h model/*.hpp)

set(CMAKE_CXX_FLAGS "-std=gnu++0x")

find_package(LibXmlPP REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(RE2 REQUIRED)

include_directories(
   ${LIBXMLPP_INCLUDE_DIR}
   ${LIBXML2_INCLUDE_DIR}
   ${RE2_INCLUDE_DIR}
)

link_directories(${LIBXMLPP_LINK_DIR})

add_definitions(${LIBXMLPP_DEFINITIONS})

add_library(${PROJECT_NAME} STATIC ${sources})

target_link_libraries(${PROJECT_NAME}
   ${LIBXMLPP_LIBRARIES}
   ${LIBXML2_LIBRARIES}
   ${Boost_REGEX_LIBRARIES}
   ${RE2_LIBRARIES}
)

# UNIT TESTS
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES Linux)
   ADD_SUBDIRECTORY(test)
endif (${CMAKE_HOST_SYSTEM_NAME} MATCHES Linux)


install(TARGETS ${PROJECT_NAME} 
   ARCHIVE
   DESTINATION lib
   COMPONENT libxsd)
   
install(FILES ${headers}
   DESTINATION include/fe/tools/ssl/xsd
   COMPONENT libxsd-dev)
