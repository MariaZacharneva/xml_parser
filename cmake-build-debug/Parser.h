//
// Created by Masha on 2024-04-29.
//

#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "XmlClasses.h"


class Parser {
public:
    Parser(const std::string& xml_input_path) {}
    void LoadTag(const XmlTag& opening_tag) {};
    void LoadText(const std::string& text) {};
};



#endif //PARSER_H
