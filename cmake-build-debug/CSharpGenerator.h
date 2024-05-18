//
// Created by Masha on 2024-05-04.
//

#ifndef CSHARPGENERATOR_H
#define CSHARPGENERATOR_H
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "XmlClasses.h"


struct ClassField {
    bool operator==(const ClassField &other) {
        return std::tie(class_name, field_name) == std::tie(other.class_name, other.field_name);
    }

    bool operator<(const ClassField &other) const {
        return std::tie(class_name, field_name) < std::tie(other.class_name, other.field_name);
    }

    std::string class_name;
    std::string field_name;
};

struct ObjectField {
    bool operator==(const ObjectField &other) {
        return std::tie(class_name, field_name, object_name) ==
               std::tie(other.class_name, other.field_name, other.object_name);
    }

    bool operator<(const ObjectField &other) const {
        return std::tie(class_name, field_name, object_name) <
               std::tie(other.class_name, other.field_name, other.object_name);
    }

    std::string class_name;
    std::string field_name;
    std::string object_name;
};

struct CS_Class {
    // bool operator==(const CS_Class &other) {
    //     return std::tie(name, name, text_fields, class_fields)
    //            == std::tie(other.name, other.name, other.text_fields, other.class_fields);
    // }

    std::string name;
    std::set<std::string> text_fields;
    std::set<ClassField> class_fields;
    std::map<std::string, int> unique_class_fields;
    // std::set<std::tuple<std::string, int, std::string>> class_fields;
};

struct CS_Object {
    // bool operator==(const CS_Object &other) {
    //     return std::tie(class_name, name, text_field_values, object_fields)
    //            == std::tie(other.class_name, other.name, other.text_field_values, other.object_fields);
    // }

    std::string class_name;
    std::string name;
    std::map<std::string, std::string> text_field_values;
    std::set<ObjectField> object_fields;
    // std::set<std::tuple<std::string, int, std::string>> object_fields;
};

class CSharpGenerator {
public:
    CSharpGenerator(const std::string &output_path) : output_path_(output_path) {
    }

    void AnalyzeTagTree(XmlTag *root);

    std::pair<CS_Class, CS_Object> ClassAndObjectFromTag(XmlTag *tag);

    void GenerateCode();

    void GenerateClass(CS_Class &class_);

    void GenerateMain();

    static std::string Capitalize(const std::string &str);

    static std::string ToLower(const std::string &str);

private:
    std::string output_path_;
    std::vector<CS_Class> cs_classes_;
    std::vector<CS_Object> cs_objects_;
};


#endif //CSHARPGENERATOR_H
