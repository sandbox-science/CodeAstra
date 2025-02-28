PROJECT    = CodeAstra
BUILD_DIR  = build
EXECUTABLE = $(PROJECT)

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
uninstall: clean
	@echo "Uninstalling the software..."
	@rm -rf $(EXECUTABLE).app $(EXECUTABLE)d.app

# Install the project
install:
	@echo "Installing $(PROJECT)..."
	@cd $(BUILD_DIR) && make
	@echo "Do you want to create a shortcut on the desktop? (Y/n)"
	@read choice; \
	if [ "$$choice" = "y" ] || [ "$$choice" = "Y" ]; then \
		echo "Creating shortcut..."; \
		OS_NAME=$(shell uname -s); \
		if [ "$$OS_NAME" = "Darwin" ]; then \
			echo "MacOS Detected..."; \
			cp -R $(EXECUTABLE).app ~/Desktop/; \
		elif [ "$$OS_NAME" = "Linux" ]; then \
			echo "Linux Detected..."; \
			cp $(EXECUTABLE) ~/Desktop/; \
		elif [ "$$OS_NAME" = "CYGWIN" ] || [ "$$OS_NAME" = "MINGW" ] || [ "$$OS_NAME" = "MSYS" ]; then \
			echo "Windows Detected..."; \
			cp $(EXECUTABLE).exe $(USERPROFILE)/Desktop/; \
		fi \
	fi
	@echo "$(PROJECT) installed."
