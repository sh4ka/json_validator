//
// Created by Jesus Flores on 05.07.24.
//

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

namespace  JSON_PARSER {
    bool parse_json(std::string json_input);
    bool is_valid_json_object(const std::string& input);
    bool is_valid_json_list(const std::string& input);
};

#endif //JSON_PARSER_H
