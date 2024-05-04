//
// Created by Masha on 2024-04-29.
//

#include "Scanner.h"

#include <set>

#include "CustomException.h"

void debug(const std::string &msg) {
    std::cout << "------- " << msg << std::endl;
}

XmlTag Scanner::ReadTag() {
    SkipEmpty();
    if (*curr_symbol_ != '<') {
        throw CustomException(kScannerException, "XML file should start with a root tag.");
    }
    XmlTag tag;
    ReadSymbol();
    if (*curr_symbol_ == '/') {
        tag.type = Closing;
        ReadSymbol();
    } else {
        tag.type = Opening;
    }
    while (isalpha(*curr_symbol_)) {
        tag.name.push_back(*curr_symbol_);
        ReadSymbol();
    }
    if (tag.name.empty()) {
        throw CustomException(kScannerException, "Tag name cannot be empty.", line_, pos_);
    }
    SkipEmpty();
    while (isalpha(*curr_symbol_)) {
        XmlAttribute attr = ReadAttribute();
        tag.attributes.push_back(attr);
    }
    if (*curr_symbol_ == '/') {
        if (tag.type == Closing) {
            throw CustomException(kScannerException, "Wrong symbol. Closing tag cannot end with '/>'.", line_, pos_);
        }
        tag.type = Empty;
        ReadSymbol();
    }
    if (*curr_symbol_ != '>') {
        throw CustomException(kScannerException, "Wrong symbol. Tag should be closed with '>'.", line_, pos_);
    }
    ReadSymbol();
    debug("read tag " + tag.name);
    return tag;
}

XmlAttribute Scanner::ReadAttribute() {
    SkipEmpty();
    XmlAttribute attribute;
    while (isalpha(*curr_symbol_)) {
        attribute.name.push_back(*curr_symbol_);
        ReadSymbol();
    }
    if (*curr_symbol_ != '=') {
        throw CustomException(kScannerException, "Attribute should have a value.", line_, pos_);
    }
    ReadSymbol();
    if (*curr_symbol_ != '"') {
        throw CustomException(kScannerException, "Attribute value should be defined.", line_, pos_);
    }
    ReadSymbol();
    while (*curr_symbol_ != '"') {
        if (!IsSymbolAllowedInTextFields(*curr_symbol_)) {
            throw CustomException(kScannerException,
                                  "Symbol '" + std::to_string(*curr_symbol_) + "' is not allowed in attribute value.",
                                  line_, pos_);
        }
        attribute.value.push_back(*curr_symbol_);
        ReadSymbol();
    }
    ReadSymbol();
    while (isspace(*curr_symbol_)) {
        ReadSymbol();
    }
    debug("read attribute " + attribute.name);
    return attribute;
}

std::string Scanner::ReadText() {
    SkipEmpty();
    std::string text;
    while (IsSymbolAllowedInTextFields(*curr_symbol_)) {
        text.push_back(*curr_symbol_);
        ReadSymbol();
    }
    debug("read text " + text);
    return text;
}

bool Scanner::CanReadFile() {
    SkipEmpty();
    return can_read_file_;
}

void Scanner::ReadSymbol() {
    if (!can_read_file_) {
        throw CustomException(kScannerException, "There are no more content in the file.", line_, pos_);
    }
    if (fin_ >> std::noskipws >> *curr_symbol_) {
        can_read_file_ = true;
        if (*curr_symbol_ == '\n') {
            line_++;
            pos_ = 0;
        } else {
            pos_++;
        }
        if (!IsSymbolAllowed(*curr_symbol_)) {
            std::cout << *curr_symbol_;
            std::string symbol;
            symbol = *curr_symbol_;
            throw CustomException(kScannerException, "Symbol '" + symbol + "' is not allowed",
                                  line_, pos_);
        }
    } else {
        can_read_file_ = false;
        debug("file content is finished");
    }
}

void Scanner::SkipEmpty() {
    while (can_read_file_ && (isspace(*curr_symbol_) || *curr_symbol_ == 0)) {
        ReadSymbol();
    }
}

bool Scanner::IsSymbolAllowed(char s) {
    std::set<char> allowed = {'-', '_', '/', '+', '-', '(', ')','!','?', '<', '>', '/', '=', '"', };
    return isalpha(s) || isdigit(s) || isspace(s)
           || allowed.find(s) != allowed.end();
}

bool Scanner::IsSymbolAllowedInTextFields(char s) {
    std::set<char> allowed = {'-', '_', '/', '+', '-', '(', ')','!','?', ' ',};
    return isalpha(s) || isdigit(s) || (allowed.find(s) != allowed.end());
}
