//
// Created by Masha on 2024-04-29.
//

#include "Parser.h"

#include "XMLParserExceptions.h"

Parser::Parser(const std::string &xml_input_path) {
}

void Parser::ParseTag() {
    Token current_token = scanner_->ReadNextToken();
    if (current_token.type != Token::OpeningBracket) {
        std::string text;
        while (current_token.type == Token::Space || current_token.type == Token::String) {
            text += current_token.content;
            current_token = scanner_->ReadNextToken();
        }
        AddTextToTree(text);
    }
    if (current_token.type != Token::OpeningBracket) {
        throw ParserException("XML file should start with a root tag.");
    }
    current_token = scanner_->ReadNextToken();
    XmlTag tag;
    if (current_token.type == Token::Slash) {
        tag.type = Closing;
        current_token = scanner_->ReadNextToken();
    } else {
        tag.type = Opening;
    }
    if (current_token.type != Token::String || current_token.content.empty()) {
        throw ParserException("Tag name cannot be empty.");
    }
    tag.name = current_token.content;
    current_token = scanner_->ReadNextToken();
    while (current_token.type == Token::Space) {
        ParseAttribute(&tag, &current_token);
    }
    if (current_token.type == Token::Slash) {
        if (tag.type == Closing) {
            throw ParserException("Wrong symbol. Closing tag cannot end with '/>'.");
        }
        tag.type = Empty;
        current_token = scanner_->ReadNextToken();
    }
    if (current_token.type != Token::ClosingBracket) {
        throw ParserException("Wrong symbol. Tag should be closed with '>'.");
    }
    AddTagToTree(tag);
}

void Parser::ParseAttribute(XmlTag *tag, Token *token) {
    if (token->type == Token::Space) {
        *token = scanner_->ReadNextToken();
    }
    if (token->type == Token::ClosingBracket || token->type == Token::Slash) {
        return;
    }
    XmlAttribute attribute;
    if (token->type == Token::String) {
        attribute.name = token->content;
    } else {
        throw ParserException("Attribute name cannot be empty.");
    }
    *token = scanner_->ReadNextToken();
    if (token->type != Token::EqualSign) {
        throw ParserException("Attribute should have a value.");
    }
    *token = scanner_->ReadNextToken();
    if (token->type != Token::Quote) {
        throw ParserException("Attribute value should be defined.");
    }
    *token = scanner_->ReadNextToken();
    while (token->type == Token::String || token->type == Token::Space) {
        attribute.value += token->content;
        *token = scanner_->ReadNextToken();
    }
    if (token->type != Token::Quote) {
        throw ParserException("Attribute value should in quotes.");
    }
    *token = scanner_->ReadNextToken();
    tag->attributes.push_back(attribute);
}

void Parser::LoadScanner(Scanner *scanner) {
    scanner_ = scanner;
}

void Parser::AddTagToTree(const XmlTag &tag) {
    if (tag.type == Opening || tag.type == Empty) {
        XmlTag *tag_ptr = new XmlTag(tag);
        if (root_tag_ == nullptr) {
            root_tag_ = tag_ptr;
            curr_tag_ = root_tag_;
        } else {
            curr_tag_->nested_tags.push_back(tag_ptr);
            tag_ptr->parent_tag = curr_tag_;
            if (tag_ptr->type == Opening) {
                curr_tag_ = tag_ptr;
            }
        }
    } else if (tag.type == Closing) {
        if (curr_tag_ == nullptr || root_tag_ == nullptr) {
            throw ParserException("Tag '" + tag.name + "' should be opened first.");
        }
        if (tag.name != curr_tag_->name) {
            throw ParserException("Tag '" + tag.name + "' is not opened within tag '" + curr_tag_->name +
                                  "', so it cannot be closed.");
        }
        curr_tag_ = curr_tag_->parent_tag;
    }
}

void Parser::AddTextToTree(const std::string &text) {
    if (curr_tag_ != nullptr && !text.empty()) {
        curr_tag_->attributes.push_back({"text", text});
    }
}
