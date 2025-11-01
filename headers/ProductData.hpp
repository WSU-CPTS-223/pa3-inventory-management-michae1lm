#ifndef PRODUCT_DATA_HPP
#define PRODUCT_DATA_HPP

#include <string>
#include <cctype>
#include "DynamicList.hpp"

struct AmazonItem {
    std::string item_id;
    std::string item_name;
    DynamicList<std::string> item_categories;
};

inline std::string strip_whitespace(const std::string& input_str) {
    size_t begin_pos = 0;
    size_t end_pos = input_str.length();

    while (begin_pos < end_pos && std::isspace(static_cast<unsigned char>(input_str[begin_pos]))) begin_pos++;
    while (end_pos > begin_pos && std::isspace(static_cast<unsigned char>(input_str[end_pos - 1]))) end_pos--;

    return input_str.substr(begin_pos, end_pos - begin_pos);
}

inline std::string convert_to_lowercase(const std::string& input_str) {
    std::string result = input_str;
    for (char& character : result) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

inline void parse_category_string(const std::string& category_input, DynamicList<std::string>& output_list) {
    if (category_input.empty()) {
        output_list.append("NA");
        return;
    }

    size_t segment_start = 0;
    size_t current_pos = 0;

    while (current_pos < category_input.length()) {
        if (category_input[current_pos] == '|') {
            std::string category_segment = strip_whitespace(category_input.substr(segment_start, current_pos - segment_start));
            if (category_segment.empty()) category_segment = "NA";
            output_list.append(category_segment);
            segment_start = current_pos + 1;
        }
        current_pos++;
    }

    std::string last_segment = strip_whitespace(category_input.substr(segment_start));
    if (last_segment.empty()) last_segment = "NA";
    output_list.append(last_segment);
}

#endif // PRODUCT_DATA_HPP
