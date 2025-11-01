#include <cassert>
#include <iostream>
#include <string>
#include "../headers/DynamicList.hpp"
#include "../headers/FastMap.hpp"
#include "../headers/ProductData.hpp"

void validate_dynamic_list() {
    std::cout << "Running DynamicList validation..." << std::endl;
    DynamicList<int> test_list;
    assert(test_list.count() == 0);
    assert(test_list.is_empty());
    test_list.append(100);
    test_list.append(200);
    test_list.append(300);
    assert(test_list.count() == 3);
    assert(test_list.access(0) == 100);
    assert(test_list.access(1) == 200);
    assert(test_list.access(2) == 300);
    try {
        test_list.access(9);
        assert(false);
    } catch (const std::out_of_range&) {}
    DynamicList<int> moved_list = std::move(test_list);
    assert(moved_list.count() == 3);
    assert(test_list.count() == 0);
    std::cout << "DynamicList validation successful!" << std::endl;
}

void validate_fast_map() {
    std::cout << "Running FastMap validation..." << std::endl;
    FastMap<std::string, double> m;
    m.store("alpha", 1.5);
    m.store("beta", 2.8);
    m.store("gamma", 3.2);
    assert(*m.retrieve("alpha") == 1.5);
    assert(*m.retrieve("beta") == 2.8);
    assert(*m.retrieve("gamma") == 3.2);
    assert(m.retrieve("nope") == nullptr);
    m.store("alpha", 9.9);
    assert(*m.retrieve("alpha") == 9.9);
    std::cout << "FastMap validation successful!" << std::endl;
}

void validate_product_data() {
    std::cout << "Running ProductData validation..." << std::endl;
    assert(strip_whitespace("  hey  ") == "hey");
    assert(convert_to_lowercase("HeLLo") == "hello");
    DynamicList<std::string> cats;
    parse_category_string("Books|Fiction|Sci-Fi", cats);
    assert(cats.count() == 3);
    assert(cats[0] == "Books");
    assert(cats[1] == "Fiction");
    assert(cats[2] == "Sci-Fi");
    cats.reset();
    parse_category_string("", cats);
    assert(cats.count() == 1);
    assert(cats[0] == "NA");
    std::cout << "ProductData validation successful!" << std::endl;
}

int main() {
    validate_dynamic_list();
    validate_fast_map();
    validate_product_data();
    std::cout << "All validations completed successfully!" << std::endl;
    return 0;
}
