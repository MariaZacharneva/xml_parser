//
// Created by Masha on 2024-04-29.
//

#include "Scanner.h"

#include <set>
#include <iostream>

#include "XMLParserExceptions.h"

void debug(const std::string &msg) {
    std::cout << "------- " << msg << std::endl;
}

Token Scanner::ReadNextToken() {
    if (*curr_symbol_ == '<') {
        ReadSymbol();
        return {Token::OpeningBracket};
    }
    if (*curr_symbol_ == '>') {
        ReadSymbol();
        return {Token::ClosingBracket};
    }
    if (*curr_symbol_ == '/') {
        ReadSymbol();
        return {Token::Slash};
    }
    if (*curr_symbol_ == '"') {
        ReadSymbol();
        return {Token::Quote};
    }
    if (*curr_symbol_ == '=') {
        ReadSymbol();
        return {Token::EqualSign};
    }
    std::string space_content;
    while (isspace(*curr_symbol_)) {
        space_content += *curr_symbol_;
        ReadSymbol();
    }

    if (!space_content.empty()) {
        return {Token::Space, space_content};
    }
    std::string content;
    while (IsSymbolAllowedInTextFields(*curr_symbol_)) {
        content += *curr_symbol_;
        ReadSymbol();
    }
    return {Token::String, content};
}

bool Scanner::CanReadFile() {
    SkipEmpty();
    return can_read_file_;
}

void Scanner::ReadSymbol() {
    if (!can_read_file_) {
        throw ScannerException("There are no more content in the file.", line_, pos_);
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
            throw ScannerException("Symbol '" + symbol + "' is not allowed",
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
    std::set<char> allowed = {'-', '_', '/', '+', '-', '(', ')', '!', '?', '<', '>', '/', '=', '"',};
    return isalpha(s) || isdigit(s) || isspace(s)
           || allowed.find(s) != allowed.end();
}

bool Scanner::IsSymbolAllowedInTextFields(char s) {
    std::set<char> allowed = {'-', '_', '/', '+', '-', '(', ')', '!', '?',};
    return isalpha(s) || isdigit(s) || (allowed.find(s) != allowed.end());
}
