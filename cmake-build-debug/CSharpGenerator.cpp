//
// Created by Masha on 2024-05-04.
//

#include "CSharpGenerator.h"

#include <algorithm>
#include <fstream>
#include <format>
#include <iostream>


void debug1(const std::string &msg) {
    std::cout << "------- " << msg << std::endl;
}

void CSharpGenerator::AnalyzeTagTree(XmlTag *root) {
    ClassAndObjectFromTag(root);
}

std::pair<CS_Class, CS_Object> CSharpGenerator::ClassAndObjectFromTag(XmlTag *tag) {
    CS_Class cs_class;
    CS_Object object;
    object.name = ToLower(tag->name);
    debug1("Analyzing tag " + tag->name);
    for (const auto &attribute: tag->attributes) {
        cs_class.text_fields.emplace(Capitalize(attribute.name));
        object.text_field_values[Capitalize(attribute.name)] = attribute.value;
    }
    for (auto nested_tag: tag->nested_tags) {
        auto nested_class_object = ClassAndObjectFromTag(nested_tag);
        cs_class.class_fields[nested_class_object.first.name] = Capitalize(nested_class_object.second.name);
        object.object_fields[nested_class_object.first.name] = nested_class_object.second.name;
    }
    auto existing_class_or = std::find(cs_classes_.begin(), cs_classes_.end(), cs_class);
    if (existing_class_or == cs_classes_.end()) {
        cs_class.name = "Class" + std::to_string(cs_classes_.size() + 1);
        cs_classes_.push_back(cs_class);
    } else {
        cs_class.name = existing_class_or->name;
    }
    object.class_name = cs_class.name;
    cs_objects_.push_back(object);
    return std::make_pair(cs_class, object);
}

void CSharpGenerator::GenerateCode() {
    for (const auto& cs_class : cs_classes_) {
        GenerateClass(cs_class);
    }
    GenerateMain();
}

void CSharpGenerator::GenerateClass(const CS_Class &class_) {
    debug1("Generating class " + class_.name);
    std::ofstream out_file(output_path_ + "/" + class_.name + ".cs");
    std::string output;
    output += "class " + class_.name + "\n{\n";
    for (const auto &text_field: class_.text_fields) {
        output += "\tpublic string " + text_field + " {get; set;} \n";
    }
    for (const auto &[class_name, name]: class_.class_fields) {
        output += "\tpublic " + class_name + " " + name + " {get; set;} \n";
    }
    output += "\tpublic " + class_.name + "(";

    for (const auto &text_field: class_.text_fields) {
        output += "string " + text_field + ", ";
    }
    for (const auto &[class_name, name]: class_.class_fields) {
        output += class_name + " " + name + ", ";
    }
    output.pop_back();
    output.pop_back();
    output += ") {/* TODO */} \n};\n";
    out_file << output;
    out_file.close();
}

void CSharpGenerator::GenerateMain() {
    debug1("Generating Main...");
    std::ofstream out_file(output_path_ + "/Main.cs");
    std::string output;
    output += "class Program {\n\tpublic static void Main(string[] args) {\n";
    for (const auto& object : cs_objects_) {
        output += "\t\t" + object.class_name + " " + object.name + " = new " + object.class_name + "(";
        for (const auto& [text_field, text] : object.text_field_values) {
            output += "\"" + text + "\", ";
        }
        for (const auto& [obj_class, obj_name] : object.object_fields) {
            output +=  obj_name + ", ";
        }
        output.pop_back();
        output.pop_back();
        output += ");\n";
    }
    output += "\t}\n}\n";
    out_file << output;
    out_file.close();
}

std::string CSharpGenerator::Capitalize(const std::string &str) {
    std::string output = str;
    for (char &s: output) {
        s = tolower(s);
    }
    output[0] = toupper(output[0]);
    return output;
}

std::string CSharpGenerator::ToLower(const std::string &str) {
    std::string output = str;
    for (char &s: output) {
        s = tolower(s);
    }
    return output;
}
