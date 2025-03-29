PROJECT    = CodeAstra
BUILD_DIR  = $(PWD)/build
EXECUTABLE = $(PROJECT)

# Set CMake options
CMAKE_OPTIONS = ..

# Default target: Run CMake and install the project
all: build install

# Run CMake to build the project
build:
	@echo "Building $(PROJECT) with CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS)

# Clean the build directory
clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

# Uninstalling the software
uninstall: clean
	@echo "Uninstalling $(PROJECT)..."
	@rm -rf $(EXECUTABLE).app $(EXECUTABLE)d.app

# Install the project
install: build
	@echo "Installing $(PROJECT)..."
	@cd $(BUILD_DIR) && make
	@echo "$(PROJECT) installed."

build_tests: build
	@cd $(BUILD_DIR)/tests/ && make

test: build_tests
	@for test in ./build/tests/test_*; do \
		if [ -f $$test ]; then \
			echo "Running $$test..."; \
			$$test; \
		fi; \
	done
