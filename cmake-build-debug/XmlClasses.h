
#ifndef XML_CLASSES_H
#define XML_CLASSES_H

#include <string>
#include <vector>

enum XmlTagType {
    Closing, Opening, Empty, Undefined,
};

struct XmlAttribute {
    std::string name;
    std::string value;
    bool last = false;
};

struct XmlTag {
    std::string name;
    std::string text;
    XmlTagType type = Undefined;

    std::vector<XmlAttribute> attributes;
    std::vector<XmlTag> nested_tags;
};

#endif  // XML_CLASSES_H