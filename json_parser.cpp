#include <regex>
#include <vector>
#include "json_parser.h"

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

bool is_true_in_loop(const std::string& str, size_t start) {
    const std::string target = "true";
    if (str.size() - start < target.size()) {
        return false;
    }
    for (size_t i = 0; i < target.size(); ++i) {
        if (str[start + i] != target[i]) {
            return false;
        }
    }
    return true;
}

bool is_false_in_loop(const std::string& str, size_t start) {
    const std::string target = "false";
    if (str.size() - start < target.size()) {
        return false;
    }
    for (size_t i = 0; i < target.size(); ++i) {
        if (str[start + i] != target[i]) {
            return false;
        }
    }
    return true;
}

bool is_null_in_loop(const std::string& str, size_t start) {
    const std::string target = "null";
    if (str.size() - start < target.size()) {
        return false;
    }
    for (size_t i = 0; i < target.size(); ++i) {
        if (str[start + i] != target[i]) {
            return false;
        }
    }
    return true;
}

bool validate_json_object_properties(const std::string& json_input) {
    bool inString = false, escape = false;
    int braceCount = 0, bracketCount = 0, quote_count = 0;
    bool expect_colon {false};
    bool expect_property {false};
    bool expect_value {false};
    bool verify_true = false;
    bool verify_false = false;

    for (size_t i = 0; i < json_input.size(); ++i) {
        char ch = json_input[i];

        if (ch == ' ' || ch == '\n') {
            continue;
        }

        if (expect_colon) {
            if (ch != ':') {
                return false;
            }
            expect_value = true;
            expect_colon = false;
            continue;
        }
        expect_colon = false;

        if (expect_value && ch == ',' && !escape && !inString) {
            expect_property = true;
            expect_value = false;
            continue;
        }

        if (expect_property && ch != '"') {
            return false;
        }

        if (ch == '{') {
            braceCount++;
            expect_property = !expect_value;
            continue;
        }

        if (ch == '}') {
            braceCount--;
            expect_property = false;
            continue;
        }

        if (ch == '[') {
            bracketCount++;
            continue;
        }

        if (ch == ']') {
            bracketCount--;
            continue;
        }

        if (ch == '"') {
            if (expect_property || expect_value) {
                inString = true;
            } else if (!escape && quote_count == 1) { // we closed our property
                expect_colon = true;
                inString = false;
            }
            if (quote_count == 0) {
                quote_count++;
            } else {
                quote_count--;
            }
            expect_property = false;
            continue;
        }

        if (ch == '\\' && inString) {
            escape = true;
            continue;
        }

        if (inString && !expect_property) {
            escape = false;
            continue;
        }

        if (expect_value) {
            if (!std::isdigit(ch)) {
                if (ch == 't' && is_true_in_loop(json_input, i)) {
                    i += 3;
                    continue;
                }
                if (ch == 'f' && is_false_in_loop(json_input, i)) {
                    i += 4;
                    continue;
                }
                if (ch == 'n' && is_null_in_loop(json_input, i)) {
                    i += 3;
                    continue;
                }
                return false;
            }
        }
    }

    return braceCount == 0 && bracketCount == 0 && quote_count == 0;
}

bool is_empty_object(const std::string& input) {
    return input.length() == 2 && input[0] == '{' && input[input.size() - 1] == '}';
}

bool is_empty_list(const std::string& input) {
    return input[0] == '[' && input[input.size() - 1] == ']';
}

bool JSON_PARSER::parse_json(std::string json_input) {
    bool valid_object = is_valid_json_object(json_input);
    bool valid_list = is_valid_json_list(json_input);
    if (valid_object) {
        // process object
        if (!is_empty_object(json_input)) {
            valid_object = validate_json_object_properties(json_input);
        }
    }
    if (valid_list && !is_empty_list(json_input)) {
        // process list
        valid_list = validate_json_object_properties(json_input);
    }
    return valid_object || valid_list;
}

bool JSON_PARSER::is_valid_json_object(const std::string& input) {
    std::string trimmedStr = input;
    trimmedStr.erase(0, trimmedStr.find_first_not_of(" \t\n\r"));
    trimmedStr.erase(trimmedStr.find_last_not_of(" \t\n\r") + 1);

    // Check if the trimmed string starts with '{' and ends with '}'
    if (trimmedStr.front() == '{' && trimmedStr.back() == '}') {
        return true;
    }
    return false;
}

bool JSON_PARSER::is_valid_json_list(const std::string &input) {
    std::string trimmedStr = input;
    trimmedStr.erase(0, trimmedStr.find_first_not_of(" \t\n\r"));
    trimmedStr.erase(trimmedStr.find_last_not_of(" \t\n\r") + 1);

    // Check if the trimmed string starts with '{' and ends with '}'
    if (trimmedStr.front() == '[' && trimmedStr.back() == ']') {
        return true;
    }
    return false;
}
