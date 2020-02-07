find_package(BISON)
find_package(FLEX)
find_package(PythonInterp)

BISON_TARGET(CPQParser cpq.y ${CMAKE_CURRENT_BINARY_DIR}/cpq.tab.cpp)
FLEX_TARGET(CPQLexer cpq.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(CPQLexer CPQParser)

add_custom_target(
        DEPENDS 
            ${CMAKE_CURRENT_BINARY_DIR}/opcodes.cpp
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/opcodes.cpp
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/opcodes.h
            ${CMAKE_CURRENT_SOURCE_DIR}/opcodes.template.cpp
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/reflector.py ${CMAKE_CURRENT_SOURCE_DIR}/opcodes ${CMAKE_CURRENT_BINARY_DIR}/opcodes.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(cpq cpq.cpp driver.cpp environment.cpp label.cpp variable.cpp ${CMAKE_CURRENT_BINARY_DIR}/opcodes.cpp ${BISON_CPQParser_OUTPUTS} ${FLEX_CPQLexer_OUTPUTS})
target_link_libraries(cpq stdc++fs)
