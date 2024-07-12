#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include "../json_parser.h"

using namespace JSON_PARSER;

std::string readFileToString(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

TEST_CASE("JSON Parsing case 0", "[json object]") {
    std::string valid_json = R"({})";
    std::string invalid_json = R"({)";

    SECTION("Check valid") {
        REQUIRE(parse_json(valid_json) == true);
    }

    SECTION("Check invalid") {
        REQUIRE(parse_json(invalid_json) == false);
    }
}

TEST_CASE("JSON Parsing case 1", "[json properties]") {
    std::string valid_json0 = R"({"key": "value"})";
    std::string valid_json1 = R"({"key": 123})";
    std::string invalid_json0 = R"({"key": "value})";
    std::string invalid_json1 = R"({"key: "value"})";
    std::string invalid_json2 = R"({"key": value})";

    SECTION("Check valid") {
        REQUIRE(parse_json(valid_json0) == true);
        REQUIRE(parse_json(valid_json1) == true);
    }

    SECTION("Check invalid") {
        REQUIRE(parse_json(invalid_json0) == false);
        REQUIRE(parse_json(invalid_json1) == false);
        REQUIRE(parse_json(invalid_json2) == false);
    }
}

TEST_CASE("JSON Parsing case 2", "[json properties]") {
    std::string valid_json0 = R"({
  "key1": true,
  "key2": false,
  "key3": null,
  "key4": "value",
  "key5": 101
})";

    SECTION("Check valid") {
        REQUIRE(parse_json(valid_json0) == true);
    }
}

TEST_CASE("JSON Parsing case 3", "[json properties]") {
    std::string valid_json0 = R"({
  "key": "value",
  "key": "value, value",
  "key-n": 101,
  "key-o": {},
  "key-l": []
})";
    std::string valid_json1 = R"({})";
    std::string valid_json2 = R"([])";
    std::string valid_json3 = R"(["",12])";

    SECTION("Check valid") {
        REQUIRE(parse_json(valid_json0) == true);
        REQUIRE(parse_json(valid_json1) == true);
        REQUIRE(parse_json(valid_json2) == true);
        REQUIRE(parse_json(valid_json3) == true);
    }
}

TEST_CASE("JSON Parsing case 4", "[json properties]") {
    std::string directory = "../test";
    std::string extension = ".json"; // Change to the desired file extension
    std::string prefix_fail = "fail";
    std::string prefix_pass = "pass";

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension && entry.path().filename().string().starts_with(prefix_pass)) {
            std::string content = readFileToString(entry.path());
            SECTION("Check valid") {
                REQUIRE(parse_json(content) == true);
            }
        }
    }

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == extension && entry.path().filename().string().starts_with(prefix_fail)) {
            std::string content = readFileToString(entry.path());
            SECTION("Check invalid") {
                REQUIRE(parse_json(content) == false);
            }
        }
    }
}