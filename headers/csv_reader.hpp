#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <string>
#include <vector>
#include <istream>

inline bool read_csv_line(std::istream& input_stream, std::vector<std::string>& output_columns) {
    output_columns.clear();
    std::string line_content;

    if (!std::getline(input_stream, line_content)) {
        return false;
    }

    enum ParseState { OutsideQuotes, InsideQuotes };
    ParseState current_state = OutsideQuotes;
    std::string current_cell;
    current_cell.reserve(128);

    for (size_t position = 0; position < line_content.length(); position++) {
        char current_char = line_content[position];

        if (current_state == OutsideQuotes) {
            if (current_char == ',') {
                output_columns.push_back(current_cell);
                current_cell.clear();
            } else if (current_char == '"') {
                current_state = InsideQuotes;
            } else {
                current_cell.push_back(current_char);
            }
        } else {
            if (current_char == '"') {
                if (position + 1 < line_content.length() && line_content[position + 1] == '"') {
                    current_cell.push_back('"');
                    position++;
                } else {
                    current_state = OutsideQuotes;
                }
            } else {
                current_cell.push_back(current_char);
            }
        }
    }

    output_columns.push_back(current_cell);
    return true;
}

#endif // CSV_READER_HPP
