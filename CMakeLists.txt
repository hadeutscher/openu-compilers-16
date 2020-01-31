cmake_minimum_required(VERSION 3.10)
project(cpq)

set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(CPQParser cpq.ypp ${CMAKE_CURRENT_BINARY_DIR}/cpq.tab.cpp)
FLEX_TARGET(CPQLexer cpq.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(CPQLexer CPQParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(cpq main.cpp ${BISON_CPQParser_OUTPUTS} ${FLEX_CPQLexer_OUTPUTS})