#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "csv_reader.hpp"
#include "../headers/DynamicList.hpp"
#include "../headers/FastMap.hpp"
#include "../headers/ProductData.hpp"

class AmazonInventory {
private:
    DynamicList<AmazonItem> all_items;
    FastMap<std::string, int> id_lookup;
    FastMap<std::string, DynamicList<int>> category_lookup;

public:
    void add_item(const AmazonItem& new_item) {
        int item_index = static_cast<int>(all_items.count());
        all_items.append(new_item);

        id_lookup.store(new_item.item_id, item_index);

        for (int i = 0; i < static_cast<int>(new_item.item_categories.count()); i++) {
            std::string category_lower = convert_to_lowercase(new_item.item_categories.access(i));
            DynamicList<int>* index_list = category_lookup.retrieve(category_lower);
            if (index_list == nullptr) {
                DynamicList<int> new_index_list;
                new_index_list.append(item_index);
                category_lookup.store(category_lower, new_index_list);
            } else {
                index_list->append(item_index);
            }
        }
    }

    const AmazonItem* locate_by_id(const std::string& search_id) const {
        const int* found_index = id_lookup.retrieve(search_id);
        if (found_index == nullptr) {
            return nullptr;
        }
        return &const_cast<AmazonInventory*>(this)->all_items.access(*found_index);
    }

    const DynamicList<int>* locate_by_category(const std::string& search_category) const {
        return category_lookup.retrieve(convert_to_lowercase(search_category));
    }

    const AmazonItem& get_item_by_index(int index) const {
        return const_cast<AmazonInventory*>(this)->all_items.access(index);
    }

    bool import_data_file(const std::string& file_path) {
        std::ifstream input_file(file_path);
        if (!input_file.is_open()) {
            std::cerr << "Failed to open data file: " << file_path << std::endl;
            return false;
        }

        std::vector<std::string> column_headers;
        if (!read_csv_line(input_file, column_headers)) {
            std::cerr << "Data file is empty" << std::endl;
            return false;
        }

        int id_column = -1, name_column = -1, category_column = -1;
        for (int i = 0; i < static_cast<int>(column_headers.size()); i++) {
            std::string header_lower = convert_to_lowercase(column_headers[i]);
            if (header_lower == "uniq_id") id_column = i;
            else if (header_lower == "product_name") name_column = i;
            else if (header_lower == "category") category_column = i;
        }

        if (id_column == -1 || name_column == -1 || category_column == -1) {
            std::cerr << "Required columns not found in data file" << std::endl;
            return false;
        }

        std::vector<std::string> data_row;
        int imported_count = 0;
        while (read_csv_line(input_file, data_row)) {
            int needed = std::max(id_column, std::max(name_column, category_column));
            if (static_cast<int>(data_row.size()) <= needed) {
                continue;
            }

            AmazonItem new_product;
            new_product.item_id = data_row[id_column];
            new_product.item_name = data_row[name_column];
            parse_category_string(data_row[column_headers.size() > category_column ? category_column : (int)column_headers.size()-1], new_product.item_categories);

            if (!new_product.item_id.empty()) {
                add_item(new_product);
                imported_count++;
            }
        }

        std::cout << "Successfully imported " << imported_count << " products" << std::endl;
        return true;
    }
};

void display_product_info(const AmazonItem& product) {
    std::cout << "Product ID: " << product.item_id << std::endl;
    std::cout << "Product Name: " << product.item_name << std::endl;
    std::cout << "Product Categories: ";
    for (int i = 0; i < static_cast<int>(product.item_categories.count()); i++) {
        std::cout << product.item_categories.access(i);
        if (i < static_cast<int>(product.item_categories.count()) - 1) {
            std::cout << " | ";
        }
    }
    std::cout << std::endl;
}

void display_product_summary(const AmazonItem& product) {
    std::cout << product.item_id << " : " << product.item_name << std::endl;
}

int main(int argc, char* argv[]) {
    std::string data_file = "amazon_dataset.csv";

    for (int i = 1; i < argc; i++) {
        std::string argument = argv[i];
        if (argument == "--data" && i + 1 < argc) {
            data_file = argv[i + 1];
            i++;
        }
    }

    AmazonInventory inventory_system;
    if (!inventory_system.import_data_file(data_file)) {
        return 1;
    }

    std::cout << "Amazon Product Inventory System Ready" << std::endl;
    std::cout << "Available commands: find <ID>, listInventory <category>, exit" << std::endl;

    std::string user_input;
    while (true) {
        std::cout << "Enter command: ";
        if (!std::getline(std::cin, user_input)) break;

        std::stringstream input_stream(user_input);
        std::string user_command;
        input_stream >> user_command;

        if (user_command == "exit" || user_command == "quit") {
            break;
        } else if (user_command == "find") {
            std::string product_id;
            input_stream >> product_id;
            const AmazonItem* found_product = inventory_system.locate_by_id(product_id);
            if (found_product == nullptr) {
                std::cout << "Product not found in inventory" << std::endl;
            } else {
                display_product_info(*found_product);
            }
        } else if (user_command == "listInventory") {
            std::string category_name;
            input_stream >> category_name;
            const DynamicList<int>* product_indices = inventory_system.locate_by_category(category_name);
            if (product_indices == nullptr || product_indices->is_empty()) {
                std::cout << "Category not found" << std::endl;
            } else {
                std::cout << "Found " << product_indices->count() << " products in category '" << category_name << "':" << std::endl;
                for (int i = 0; i < static_cast<int>(product_indices->count()); i++) {
                    const AmazonItem& product = inventory_system.get_item_by_index(product_indices->access(i));
                    display_product_summary(product);
                }
            }
        } else {
            std::cout << "Unrecognized command: " << user_command << std::endl;
        }
    }

    return 0;
}
