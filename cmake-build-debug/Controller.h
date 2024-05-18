//
// Created by Masha on 2024-04-29.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>

#include "CSharpGenerator.h"
#include "XMLParserExceptions.h"
#include "Parser.h"
#include "Scanner.h"


class Controller {
public:
    Controller(const std::string &xml_input_path, const std::string &output_path) {
        parser_ = std::make_unique<Parser>(xml_input_path);
        scanner_ = std::make_unique<Scanner>(xml_input_path);
        generator_ = std::make_unique<CSharpGenerator>(output_path);
    }

    void Run() {
        parser_->LoadScanner(scanner_.get());
        try {
            can_read_file_ = scanner_->CanReadFile();
            while (can_read_file_) {
                parser_->ParseTag();
                can_read_file_ = scanner_->CanReadFile();
            }
            generator_->AnalyzeTagTree(parser_->GetRoot());
            generator_->GenerateCode();
        } catch (std::exception& error) {
            // Different handling of differnet expections can be added if necessary.
            std::cout << "AN ERROR OCCURED.\n" << error.what() << std::endl;
        }
    }

private:
    std::unique_ptr<Parser> parser_;
    std::unique_ptr<Scanner> scanner_;
    std::unique_ptr<CSharpGenerator> generator_;
    bool can_read_file_ = true;
};


#endif //CONTROLLER_H
