//
// Created by Masha on 2024-04-29.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>

#include "CSharpGenerator.h"
#include "CustomException.h"
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
        try {
            can_read_file_ = scanner_->CanReadFile();
            while (can_read_file_) {
                auto tag = scanner_->ReadTag();
                parser_->LoadTag(tag);
                can_read_file_ = scanner_->CanReadFile();
                if (!can_read_file_) {
                    break;
                }
                std::string text = scanner_->ReadText();
                parser_->LoadText(text);
                can_read_file_ = scanner_->CanReadFile();
            }
            generator_->AnalyzeTagTree(parser_->GetRoot());
            generator_->GenerateCode();
        } catch (CustomException& error) {
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
