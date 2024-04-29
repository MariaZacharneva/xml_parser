//
// Created by Masha on 2024-04-29.
//

#include "Scanner.h"

void debug(const std::string& msg) {
    std::cout << "-------" << msg << std::endl;
}

XmlTag Scanner::ReadOpeningTag() {
    debug("Reading opening tag ...");
    while (*_curr_symbol != '<') {
        ReadSymbol();
    }
    XmlTag tag;
    ReadSymbol();
    while (isalpha(*_curr_symbol)) {
        tag.name.push_back(*_curr_symbol);
        ReadSymbol();
    }
    while (isspace(*_curr_symbol)) {
        ReadSymbol();
    }
    while (isalpha(*_curr_symbol)) {
        XmlAttribute attr = ReadAttribute();
        tag.attributes.push_back(attr);
    }
    if (*_curr_symbol == '/') {
        tag.empty = true;
        ReadSymbol();
        if (*_curr_symbol != '>') {
            // TODO: error
        }
    }
    if (*_curr_symbol == '>') {
        return tag;
    } else {
        // TODO: error
    }
}

XmlAttribute Scanner::ReadAttribute() {
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
        attribute.value.push_back(*_curr_symbol);
        ReadSymbol();
    }
    ReadSymbol();
    while (isspace(*_curr_symbol)) {
        *_curr_symbol = ReadSymbol();
    }
    return attribute;
}

char Scanner::ReadSymbol() {
    if (fin >> std::noskipws >> *_curr_symbol) {
        std::cout << *_curr_symbol << std::endl;
        return *_curr_symbol;
    }
    debug("not all good");
    throw std::exception();
    // TODO: send error message or file has ended
    return char();
}