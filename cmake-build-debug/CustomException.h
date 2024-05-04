//
// Created by Masha on 2024-04-30.
//

#ifndef ERROR_H
#define ERROR_H
#include <exception>
#include <string>

const std::string kScannerException = "Scanner exception";
const std::string kParserException = "Parser exception";


class CustomException : public std::exception {
public:
    CustomException(const std::string &error_type, const std::string &message, int line = 0, int pos = 0)
        : type_(error_type),
          message_(message), line_(line), pos_(pos) {
    }

    char *what() {
        if (line_ != 0 || pos_ != 0) {
            output_ = type_ + " line " + std::to_string(line_) + " pos " + std::to_string(pos_) + ": " + message_;
        } else {
            output_ = type_ + ": " + message_;
        }
        return output_.data();
    }

private:
    std::string type_;
    std::string message_;
    std::string output_;
    int line_;
    int pos_;
};


#endif //ERROR_H
