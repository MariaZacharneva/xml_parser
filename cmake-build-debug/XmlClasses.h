
#ifndef XML_CLASSES_H
#define XML_CLASSES_H

#include <string>
#include <vector>

struct XmlAttribute {
    std::string name;
    std::string value;
};

struct XmlTag {
    std::string name;
    std::string text;
    bool empty = false;
    std::vector<XmlAttribute> attributes;
    std::vector<XmlTag> nested_tags;
};

#endif  // XML_CLASSES_H