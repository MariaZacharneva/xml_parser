
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

class XmlTag {
public:
    ~XmlTag() {
        for (XmlTag* child : nested_tags) {
            delete child;
        }
    }
    std::string name;
    std::string text;
    XmlTagType type = Undefined;

    std::vector<XmlAttribute> attributes;
    std::vector<XmlTag*> nested_tags;
    XmlTag* parent_tag = nullptr;
};

#endif  // XML_CLASSES_H