cmake_minimum_required(VERSION 3.28)
project(GoogleTest)

set(GOOGLETEST_VERSION 1.9.0)
# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(lib/googletest/include)

# 'Google_Tests_run' is the target name
# 'Test1_EmptyTag.cpp Test2_TagWithText.cpp' are source files with tests
add_executable(GoogleTest_run test.cpp
        ../Controller.h
        ../Scanner.cpp
        ../Scanner.h
        ../Parser.cpp
        ../Parser.h
        ../XmlClasses.h
        ../XMLParserExceptions.h
        ../CSharpGenerator.cpp
        ../CSharpGenerator.h)
target_link_libraries(GoogleTest_run gtest gtest_main)