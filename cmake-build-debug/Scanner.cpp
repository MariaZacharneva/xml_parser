//
// Created by Masha on 2024-04-29.
//

#include "Scanner.h"

#include "helpers.h"

void debug(const std::string &msg) {
    std::cout << "-------" << msg << std::endl;
}

XmlTag Scanner::ReadTag() {
    debug("Reading tag ...");
    SkipEmpty();
    if (*_curr_symbol != '<') {
        // TODO: error
    }
    XmlTag tag;
    ReadSymbol();
    if (*_curr_symbol == '/') {
        tag.type = Closing;
        ReadSymbol();
    } else {
        tag.type = Opening;
    }
    debug("Not dead yet");
    while (isalpha(*_curr_symbol)) {
        tag.name.push_back(*_curr_symbol);
        ReadSymbol();
    }
    SkipEmpty();
    while (isalpha(*_curr_symbol)) {
        XmlAttribute attr = ReadAttribute();
        tag.attributes.push_back(attr);
    }
    if (*_curr_symbol == '/') {
        if (tag.type == Closing) {
            // TODO: error
        }
        tag.type = Empty;
        ReadSymbol();
    }
    if (*_curr_symbol == '>') {
        ReadSymbol();
        return tag;
    } else {
        // TODO: error
    }
}

XmlAttribute Scanner::ReadAttribute() {
    SkipEmpty();
    debug("Reading attribute...");
    XmlAttribute attribute;
    while (isalpha(*_curr_symbol)) {
        attribute.name.push_back(*_curr_symbol);
        ReadSymbol();
    }
    debug(attribute.name);
    if (*_curr_symbol != '=') {
        // TODO: error
    }
    ReadSymbol();
    if (*_curr_symbol != '"') {
        // TODO: error
    }
    ReadSymbol();
    while (*_curr_symbol != '"') {
        if (!IsSymbolAllowed(*_curr_symbol)) {
            // TODO: error
        }
        attribute.value.push_back(*_curr_symbol);
        ReadSymbol();
    }
    ReadSymbol();
    while (isspace(*_curr_symbol)) {
        ReadSymbol();
    }
    return attribute;
}

std::string Scanner::ReadText() {
    SkipEmpty();
    std::string text;
    while (IsSymbolAllowed(*_curr_symbol)) {
        text.push_back(*_curr_symbol);
    }
    return text;
}

bool Scanner::CanReadFile() {
    SkipEmpty();
    return can_read_file;
}

void Scanner::ReadSymbol() {
    if (!can_read_file) {
        // TODO: error
        debug("not all good");
        throw std::exception();
    }
    if (fin >> std::noskipws >> *_curr_symbol) {
        can_read_file = true;
        std::cout << *_curr_symbol << std::endl;
    } else {
        can_read_file = false;
        debug("file content is finished");
    }
}

void Scanner::SkipEmpty() {
    while (can_read_file && (isspace(*_curr_symbol) || *_curr_symbol == 0)) {
        ReadSymbol();
    }
}

bool Scanner::IsSymbolAllowed(char s) {
    return isalpha(s) || isdigit(s) || s == '-' || s == '_' || s == '/' || s == '+' || s == '(' || s == ')';
}