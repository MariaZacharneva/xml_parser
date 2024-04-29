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
    Scanner(const std::string &xml_input_file) : _xml_input_path(xml_input_file),
                                                 fin(xml_input_file, std::fstream::in), _curr_symbol(new char(0)) {
    }

    XmlTag ReadOpeningTag();

    XmlAttribute ReadAttribute();

private:
    char ReadSymbol();

    std::string _xml_input_path;
    std::fstream fin;
    char *_curr_symbol;
};


#endif //SCANNER_H
