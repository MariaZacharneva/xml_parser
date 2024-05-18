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
        cs_class.class_fields.insert({nested_class_object.first.name, Capitalize(nested_class_object.second.name)});
        object.object_fields.insert({
            nested_class_object.first.name, Capitalize(nested_class_object.second.name), nested_class_object.second.name
        });
    }
    auto existing_class_or =
            std::find_if(cs_classes_.begin(), cs_classes_.end(),
                         [&cs_class](const auto &item) {
                             return (std::includes(item.text_fields.begin(), item.text_fields.end(),
                                                   cs_class.text_fields.begin(), cs_class.text_fields.end())
                                     && std::includes(item.class_fields.begin(), item.class_fields.end(),
                                                      cs_class.class_fields.begin(), cs_class.class_fields.end())
                                     || (std::includes(cs_class.text_fields.begin(), cs_class.text_fields.end(),
                                                       item.text_fields.begin(), item.text_fields.end())
                                         && std::includes(cs_class.class_fields.begin(), cs_class.class_fields.end(),
                                                          item.class_fields.begin(), item.class_fields.end())
                                     ));
                         });
    if (existing_class_or == cs_classes_.end()) {
        cs_class.name = "Class" + std::to_string(cs_classes_.size() + 1);
        cs_classes_.push_back(cs_class);
    } else {
        for (auto &class_field: cs_class.class_fields) {
            existing_class_or->class_fields.insert(class_field);
        }
        for (auto &text_field: cs_class.text_fields) {
            existing_class_or->text_fields.insert(text_field);
        }
        cs_class.name = existing_class_or->name;
    }
    object.class_name = cs_class.name;
    cs_objects_.push_back(object);
    return std::make_pair(cs_class, object);
}

void CSharpGenerator::GenerateCode() {
    for (auto &cs_class: cs_classes_) {
        for (auto &[class_name, _]: cs_class.class_fields) {
            cs_class.unique_class_fields[class_name]++;
        }
        GenerateClass(cs_class);
    }
    GenerateMain();
}

void CSharpGenerator::GenerateClass(CS_Class &class_) {
    debug1("Generating class " + class_.name);
    std::ofstream out_file(output_path_ + "/" + class_.name + ".cs");
    std::string output;
    output += "class " + class_.name + "\n{\n";
    for (const auto &text_field: class_.text_fields) {
        output += "\tpublic string " + text_field + " {get; set;} \n";
    }
    for (const auto &[class_name, name]: class_.class_fields) {
        if (class_.unique_class_fields.at(class_name) == 1) {
            output += "\tpublic " + class_name + " " + name + " {get; set;} \n";
        } else if (class_.unique_class_fields.at(class_name) > 1) {
            output += "\tpublic List<" + class_name + "> " + class_name + "Objects {get; set;} \n";
            class_.unique_class_fields[class_name] *= -1;
        }
    }
    output += "\tpublic " + class_.name + "(";
    for (const auto &text_field: class_.text_fields) {
        output += "string " + text_field + ", ";
    }
    for (const auto &[class_name, name]: class_.class_fields) {
        if (class_.unique_class_fields.at(class_name) == 1) {
            output += class_name + " " + name + ", ";
        } else if (class_.unique_class_fields.at(class_name) < 1) {
            output += "List<" + class_name + "> " + class_name + "Objects, ";
            class_.unique_class_fields[class_name] *= -1;
        }
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
    for (const auto &object: cs_objects_) {
        auto class_ = std::find_if(cs_classes_.begin(), cs_classes_.end(),
                                   [&object](const CS_Class &class_1) {
                                       return class_1.name == object.class_name;
                                   });

        for (auto &[name, counter]: class_->unique_class_fields) {
            if (counter > 1) {
                output += "\t\tList<" + name + "> " + name + "_objects = new List<" + name +
                        ">(" + std::to_string(counter) + ");\n";
                for (auto& field : object.object_fields) {
                    if (field.class_name == name) {
                        output += "\t\t" + name + "_objects.Add(" + field.object_name + ");\n";
                    }
                }
            }
        }
        output += "\t\t" + object.class_name + " " + object.name + " = new " + object.class_name + "(";
        for (const auto &text_field: class_->text_fields) {
            std::string field_value;
            if (object.text_field_values.find(text_field) != object.text_field_values.end()) {
                field_value = object.text_field_values.at(text_field);
                output += "\"" + field_value + "\", ";
            } else {
                field_value = "null";
                output += field_value + ", ";
            }
        }
        for (const auto &class_field: class_->class_fields) {
            auto field = std::find_if(object.object_fields.begin(), object.object_fields.end(),
                                      [&class_field](const ObjectField &item) {
                                          return class_field.class_name == item.class_name && class_field.field_name ==
                                                 item.field_name;
                                      });
            if (field != object.object_fields.end()) {
                if (class_->unique_class_fields[field->class_name] > 1) {
                    output += field->class_name + "_objects,";
                    class_->unique_class_fields[field->class_name] *= -1;
                } else if (class_->unique_class_fields[field->class_name] == 1) {
                    output += field->object_name + ", ";
                }
            } else {
                output += "null, ";
            }
        }
        output.pop_back();
        output.pop_back();
        output += ");\n";
    }
    output += "\t}\n}\n";
    out_file << output;
    out_file.close();
}


// add some interpretation of a collection if we have several object of the same class.
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
