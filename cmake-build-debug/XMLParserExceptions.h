#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

const std::string kScannerException = "Scanner exception";
const std::string kParserException = "Parser exception";


class ScannerException : public std::exception {
public:
    ScannerException(const std::string &message, int line = 0, int pos = 0)
        : message_(message), line_(line), pos_(pos) {
        output_ = "Scanner Exception: line " + std::to_string(line_) + " pos " + std::to_string(pos_) + ": " + message_;
    }

    virtual const char* what() const noexcept {
        return output_.c_str();
    }

private:
    std::string message_;
    std::string output_;
    int line_;
    int pos_;
};

class ParserException : public std::exception {
public:
    ParserException(const std::string &message): message_(message) {
    }

    char *what() {
        message_.data();
    }

private:
    std::string message_;
};


#endif //ERROR_H
