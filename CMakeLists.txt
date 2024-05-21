cmake_minimum_required(VERSION 3.28)
project(xml_parser)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(cmake-build-debug/GoogleTest)

add_executable(xml_parser parser_main.cpp
        cmake-build-debug/Controller.h
        cmake-build-debug/Scanner.cpp
        cmake-build-debug/Scanner.h
        cmake-build-debug/Parser.cpp
        cmake-build-debug/Parser.h
        cmake-build-debug/XmlClasses.h
        cmake-build-debug/XMLParserExceptions.h
        cmake-build-debug/CSharpGenerator.cpp
        cmake-build-debug/CSharpGenerator.h
)
