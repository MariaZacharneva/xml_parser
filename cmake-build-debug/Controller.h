//
// Created by Masha on 2024-04-29.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>

#include "Parser.h"
#include "Scanner.h"


class Controller {
public:
    Controller(const std::string& xml_input_path) {
        _parser = std::make_unique<Parser>(xml_input_path);
        _scanner = std::make_unique<Scanner>(xml_input_path);
    }
    void Run() {
        _scanner->ReadOpeningTag();
    }
private:
    std::unique_ptr<Parser> _parser;
    std::unique_ptr<Scanner> _scanner;
};



#endif //CONTROLLER_H
