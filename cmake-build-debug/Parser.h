//
// Created by Masha on 2024-04-29.
//

#ifndef PARSER_H
#define PARSER_H
#include <queue>
#include <string>

#include "Scanner.h"
#include "XmlClasses.h"

enum Stage {
    BuildingOpeningTag,
};

class Parser {
public:
    Parser(const std::string &xml_input_path);

    ~Parser() { delete root_tag_; }

    void ParseTag();

    void ParseAttribute(XmlTag* tag, Token* token);

    void LoadScanner(Scanner* scanner);

    void AddTagToTree(const XmlTag &tag);

    void AddTextToTree(const std::string &text);

    [[nodiscard]] XmlTag *GetRoot() const { return root_tag_; }

private:
    Scanner* scanner_;
    XmlTag *root_tag_ = nullptr;
    XmlTag *curr_tag_ = nullptr;
};


#endif //PARSER_H
