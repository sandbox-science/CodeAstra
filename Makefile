PROJECT    = CodeAstra
BUILD_DIR  = build
EXECUTABLE = $(PROJECT).app

# Set CMake options
CMAKE_OPTIONS = ..

# Default target: Run CMake and install the project
all: configure install

# Run CMake to configure the project
configure:
	@echo "Configuring project with CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake $(CMAKE_OPTIONS)

# Clean the build directory
clean:
	@echo "Cleaning the build directory..."
	@rm -rf $(BUILD_DIR)

# Uninstalling the software
uninstall:
	@echo "Uninstalling the software..."
	@rm -rf $(EXECUTABLE)

# Install the project
install:
	@echo "Installing $(PROJECT)..."
	@cd $(BUILD_DIR) && make
