find_package(BISON)
find_package(FLEX)

BISON_TARGET(CPQParser cpq.y ${CMAKE_CURRENT_BINARY_DIR}/cpq.tab.cpp)
FLEX_TARGET(CPQLexer cpq.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(CPQLexer CPQParser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(cpq cpq.cpp driver.cpp environment.cpp label.cpp variable.cpp ${BISON_CPQParser_OUTPUTS} ${FLEX_CPQLexer_OUTPUTS})
target_link_libraries(cpq stdc++fs)
