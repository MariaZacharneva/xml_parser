//
// Created by Masha on 2024-05-21.
//
#include <cmath>
#include <filesystem>
#include <gtest/gtest.h>
#include "fstream"
#include "../../cmake-build-debug/Controller.h"
#include "../../cmake-build-debug/Parser.h"
#include "../../cmake-build-debug/Scanner.h"
#include "../../cmake-build-debug/CSharpGenerator.h"


bool FileExists(const std::string &name) {
    std::ifstream f(name.c_str());
    return f.good();
}

const std::string kPath = "../../GoogleTest/";

void ExecuteTest(const std::string &xml_path, const std::string &input, const std::string &output_path,
                 const std::vector<std::string> &generated_files,
                 const std::vector<std::string> &expected_files) {
    ASSERT_EQ(generated_files.size(), expected_files.size());
    auto dirIter = std::filesystem::directory_iterator(output_path);
    // Creating input file
    std::ofstream input_stream(xml_path);
    input_stream << input << std::endl;
    input_stream.flush();
    input_stream.close();
    ASSERT_TRUE(FileExists(xml_path));

    // Cleaning files for generation
    for (auto &file: generated_files) {
        std::ofstream temp(file);
        temp << std::endl;
    }

    auto controller = std::make_unique<Controller>(xml_path, output_path);
    controller->Run();

    int fileCount = std::count_if(
        begin(dirIter),
        end(dirIter),
        [](auto &entry) { return entry.is_regular_file(); }
    );
    EXPECT_EQ(fileCount, generated_files.size() + expected_files.size() + 1);

    for (int i = 0; i < generated_files.size(); i++) {
        ASSERT_TRUE(FileExists(generated_files[i]));
        ASSERT_TRUE(FileExists(expected_files[i]));

        std::ifstream f1(generated_files[i]);
        std::string gen_file((std::istreambuf_iterator<char>(f1)),
                             std::istreambuf_iterator<char>());

        std::ifstream f2(expected_files[i]);
        std::string exp_file((std::istreambuf_iterator<char>(f2)),
                             std::istreambuf_iterator<char>());

        EXPECT_EQ(exp_file, gen_file);
    }
}

void ExecuteExceptionTest(const std::string &xml_path, const std::string &input, const std::string &output_path) {
    // Creating input file
    std::ofstream input_stream(xml_path);
    input_stream << input << std::endl;
    input_stream.flush();
    input_stream.close();
    ASSERT_TRUE(FileExists(xml_path));

    auto controller = std::make_unique<Controller>(xml_path, output_path);
    controller->Run(true);
}

TEST(XMLParserTest_CorrectInput, Test1_EmptyTag) {
    std::string prefix_path = kPath + "Test1_EmptyTag";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {prefix_path + "/Class1.cs", prefix_path + "/Main.cs"};
    std::vector<std::string> expected_files = {prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedMain.cs"};

    std::string input = "<cat Name=\"Pies\"/>";

    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_CorrectInput, Test2_TagWithText) {
    std::string prefix_path = kPath + "Test2_TagWithText";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {
        prefix_path + "/Class1.cs", prefix_path + "/Main.cs"
    };
    std::vector<std::string> expected_files = {
        prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedMain.cs"
    };

    std::string input = "<message To=\"Mum\">Hello</message>";
    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_CorrectInput, Test3_NestedTagsWithText) {
    std::string prefix_path = kPath + "Test3_NestedTagsWithText";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {
        prefix_path + "/Class1.cs", prefix_path + "/Class2.cs", prefix_path + "/Class3.cs", prefix_path + "/Main.cs"
    };
    std::vector<std::string> expected_files = {
        prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedClass2.cs", prefix_path + "/ExpectedClass3.cs",
        prefix_path + "/ExpectedMain.cs"
    };

    std::string input =
            "<root>\n"
            "<message To=\"Mum\">Hello</message>\n"
            "<call From=\"Daughter\" Day=\"today\">Hello</call>\n"
            "</root>";
    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_CorrectInput, Test4_CombineFieldsToOneClass) {
    std::string prefix_path = kPath + "Test4_CombineFieldsToOneClass";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {
        prefix_path + "/Class1.cs", prefix_path + "/Class2.cs", prefix_path + "/Class3.cs", prefix_path + "/Class4.cs",
        prefix_path + "/Main.cs"
    };
    std::vector<std::string> expected_files = {
        prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedClass2.cs", prefix_path + "/ExpectedClass3.cs",
        prefix_path + "/ExpectedClass4.cs",
        prefix_path + "/ExpectedMain.cs"
    };

    std::string input =
            "<root>\n"
            "<A>\n"
            "<message To=\"Mum\">Hello</message>\n"
            "</A>\n"
            "<B>\n"
            "<call To=\"Dad\" From=\"Daughter\">Hi!</call>\n"
            "</B>\n"
            "</root>";
    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_CorrectInput, Test5_CollectionOfObjects) {
    std::string prefix_path = kPath + "Test5_CollectionOfObjects";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {
        prefix_path + "/Class1.cs", prefix_path + "/Class2.cs", prefix_path + "/Class3.cs", prefix_path + "/Main.cs"
    };
    std::vector<std::string> expected_files = {
        prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedClass2.cs", prefix_path + "/ExpectedClass3.cs",
        prefix_path + "/ExpectedMain.cs"
    };

    std::string input =
            "<root>\n"
            "<queue To=\"Shop\">\n"
            "<man1 Name=\"Jan\">Hello</man1>\n"
            "<man2 Name=\"Pawel\">Hi</man2>\n"
            "<man3 Name=\"TheSecond\" Surname=\"Pope\">Good day!</man3>\n"
            "</queue>\n"
            "</root>";
    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_CorrectInput, Test6_ComplexCase) {
    std::string prefix_path = kPath + "Test6_ComplexCase";

    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::vector<std::string> generated_files = {
        prefix_path + "/Class1.cs", prefix_path + "/Class2.cs", prefix_path + "/Class3.cs", prefix_path + "/Class4.cs",
        prefix_path + "/Main.cs"
    };
    std::vector<std::string> expected_files = {
        prefix_path + "/ExpectedClass1.cs", prefix_path + "/ExpectedClass2.cs", prefix_path + "/ExpectedClass3.cs",
        prefix_path + "/ExpectedClass4.cs",
        prefix_path + "/ExpectedMain.cs"
    };

    std::string input =
            "<root>\n"
            "<newman Name=\"John\" Surname=\"Newman\" BirthDate=\"1990-01-01\">\n"
            "<mother Name=\"Ann\" Surname=\"Brown\" BirthDate=\"1970-02-02\"/>\n"
            "</newman>\n"
            "<novak Name=\"Matt\" Surname=\"Novak\" BirthDate=\"2005-11-16\">\n"
            "<mother Name=\"mARY\" Surname=\"Novak\" BirthDate=\"1970-02-02\"/>\n"
            "<father Name=\"Henry\" Surname=\"Novak\" BirthDate=\"1970-02-02\"/>\n"
            "</novak>\n"
            "<cats>\n"
            "<cat1 Name=\"Tom\" Companion=\"Jerry\"/>\n"
            "<cat2 Name=\"Garfield\" Companion=\"Odie\"/>\n"
            "</cats>\n"
            "</root>";
    EXPECT_NO_THROW({
        ExecuteTest(xml_path, input, output_path, generated_files, expected_files);
        });
}

TEST(XMLParserTest_ScannerException, Test7_NoContent) {
    std::string prefix_path = kPath + "Test7_NoContent";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root>Hello!</roo";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ScannerException);
}

TEST(XMLParserTest_ScannerException, Test8_SymbolNotAllowed) {
    std::string prefix_path = kPath + "Test8_SymbolNotAllowed";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root>Hello@</roo";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ScannerException);
}

TEST(XMLParserTest_ParserException, Test9_NoRootTag) {
    std::string prefix_path = kPath + "Test9_NoRootTag";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "Hello! <root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
    input = "===";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test10_EmptyTagName) {
    std::string prefix_path = kPath + "Test10_EmptyTagName";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<>Hello!</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test11_DoubleClosedTag) {
    std::string prefix_path = kPath + "Test11_DoubleClosedTag";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root>Hello!</root/>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test12_TagNotClosed) {
    std::string prefix_path = kPath + "Test12_TagNotClosed";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root>Hello!\n"
                        "<message>\n"
                        "</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test13_EmptyAttributeName) {
    std::string prefix_path = kPath + "Test13_EmptyAttributeName";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root =\"Start\">Hello!</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test14_EmptyAttributeValue) {
    std::string prefix_path = kPath + "Test14_EmptyAttributeValue";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root status>Hello!</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test15_AttributeValueNotDefined) {
    std::string prefix_path = kPath + "Test15_AttributeValueNotDefined";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root status=>Hello!</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test16_AttributeValueNoQuotes) {
    std::string prefix_path = kPath + "Test16_AttributeValueNoQuotes";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;;
    std::string input = "<root status=Start>Hello!</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test17_TagNotOpened) {
    std::string prefix_path = kPath + "Test17_TagNotOpened";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}

TEST(XMLParserTest_ParserException, Test18_TagNotOpened2) {
    std::string prefix_path = kPath + "Test18_TagNotOpened2";
    std::string xml_path = prefix_path + "/test.xml";
    std::string output_path = prefix_path;
    std::string input = "<root>Hello!\n"
                        "</message>\n"
                        "</root>";
    EXPECT_THROW(ExecuteExceptionTest(xml_path, input, output_path), ParserException);
}