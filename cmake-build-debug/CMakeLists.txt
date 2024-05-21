cmake_minimum_required(VERSION 3.28)
project(xml_parser_src)

set(CMAKE_CXX_STANDARD 17)

add_executable(xml_parser_src
        Controller.h
        Scanner.cpp
        Scanner.h
        Parser.cpp
        Parser.h
        XmlClasses.h
        XMLParserExceptions.h
        CSharpGenerator.cpp
        CSharpGenerator.h
)