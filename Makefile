DEBUG      ?= OFF
PROJECT    = CodeAstra
BUILD_DIR  = $(PWD)/build

CMAKE_OPTIONS = ..

.PHONY: all build clean install build_tests test

all: install

build:
	@echo "Building $(PROJECT)..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS) -DDEBUG=$(DEBUG)

clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

install: build
	@echo "Installing $(PROJECT)..."
	@cmake --build $(BUILD_DIR)
	@echo "Installation complete."

build_tests: build
	@echo "Building tests..."
	@$(MAKE) -C $(BUILD_DIR)/tests

test: build_tests
	@echo "Running tests..."
	@for test in ./build/tests/test_*; do \
		if [ -f $$test ]; then \
			echo "Running $$test..."; \
			$$test; \
		fi; \
	done

run:
	@echo "Running $(PROJECT)..."
	@./build/bin/$(PROJECT)
