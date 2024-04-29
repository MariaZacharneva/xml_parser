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
    Controller(const std::string &xml_input_path) {
        _parser = std::make_unique<Parser>(xml_input_path);
        _scanner = std::make_unique<Scanner>(xml_input_path);
    }

    void Run() {
        _can_read_file = _scanner->CanReadFile();
        while (_can_read_file) {
            auto tag = _scanner->ReadTag();
            _parser->LoadTag(tag);
            _can_read_file = _scanner->CanReadFile();
            if (!_can_read_file) {
                break;
            }
            std::string text = _scanner->ReadText();
            _parser->LoadText(text);
            _can_read_file = _scanner->CanReadFile();
        }
    }

private:
    std::unique_ptr<Parser> _parser;
    std::unique_ptr<Scanner> _scanner;
    bool _can_read_file = true;
};


#endif //CONTROLLER_H
