//
// Created by Masha on 2024-04-29.
//

#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <fstream>
#include <iostream>

#include "XmlClasses.h"


class Scanner {
public:
    Scanner(const std::string &xml_input_file) : xml_input_path_(xml_input_file),
                                                 fin_(xml_input_file, std::fstream::in), curr_symbol_(new char(0)) {
    }

    Token ReadNextToken();

    bool CanReadFile();

private:
    void ReadSymbol();

    void SkipEmpty();

    static bool IsSymbolAllowed(char s);

    static bool IsSymbolAllowedInTextFields(char s);

    std::string xml_input_path_;
    std::fstream fin_;
    int line_ = 1;
    int pos_ = 1;
    char *curr_symbol_;
    bool can_read_file_ = true;
};


#endif //SCANNER_H
