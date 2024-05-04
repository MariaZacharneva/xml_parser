//
// Created by Masha on 2024-04-29.
//

#include "Parser.h"

#include "CustomException.h"

Parser::Parser(const std::string &xml_input_path) {
}

void Parser::LoadTag(const XmlTag &tag) {
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
            throw CustomException(kParserException, "Tag '" + tag.name + "' should be opened first.");
        }
        if (tag.name != curr_tag_->name) {
            throw CustomException(kParserException,
                                  "Tag '" + tag.name + "' is not opened within tag '" + curr_tag_->name +
                                  "', so it cannot be closed.");
        }
        curr_tag_ = curr_tag_->parent_tag;
    }
}

void Parser::LoadText(const std::string &text) {
    if (curr_tag_ != nullptr && !text.empty()) {
        curr_tag_->attributes.push_back({"text", text});
    }
}
