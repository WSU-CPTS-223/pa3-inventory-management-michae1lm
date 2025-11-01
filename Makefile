COMPILER = g++
FLAGS = -std=c++11 -Wall -Wextra
INCLUDE_PATHS = -Iheaders
OUTPUT_DIR = build
MAIN_APP = $(OUTPUT_DIR)/amazon_manager
TEST_APP = $(OUTPUT_DIR)/test_suite
MAIN_SOURCE = src/app_runner.cpp
TEST_SOURCE = tests/validation.cpp

default: $(MAIN_APP)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

$(MAIN_APP): $(OUTPUT_DIR) $(MAIN_SOURCE)
	$(COMPILER) $(FLAGS) $(INCLUDE_PATHS) -o $(MAIN_APP) $(MAIN_SOURCE)

check: $(OUTPUT_DIR) $(TEST_SOURCE)
	$(COMPILER) $(FLAGS) $(INCLUDE_PATHS) -o $(TEST_APP) $(TEST_SOURCE)

remove:
	rm -rf $(OUTPUT_DIR)
