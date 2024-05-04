//
// Created by Masha on 2024-04-29.
//

#ifndef PARSER_H
#define PARSER_H
#include <string>

#include "XmlClasses.h"


class Parser {
public:
    Parser(const std::string &xml_input_path);

    ~Parser() { delete root_tag_; }

    void LoadTag(const XmlTag &tag);

    void LoadText(const std::string &text);

    [[nodiscard]] XmlTag *GetRoot() const { return root_tag_; }

private:
    XmlTag *root_tag_ = nullptr;
    XmlTag *curr_tag_ = nullptr;
};


#endif //PARSER_H
