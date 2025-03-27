PROJECT    = CodeAstra
BUILD_DIR  = $(PWD)/build
EXECUTABLE = $(PROJECT)

# Set CMake options
CMAKE_OPTIONS = ..

# Default target: Run CMake and install the project
all: build install

# Run CMake to build the project
build:
	@echo "Building project with CMake..."
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
	@OS_NAME=$(shell uname -s 2>/dev/null || echo "Windows"); \
	if [ "$$OS_NAME" = "Darwin" ]; then \
		rm -rf "$(HOME)/Desktop/$(EXECUTABLE).app"; \
		echo "MacOS: Shortcut removed..."; \
	elif [ "$$OS_NAME" = "Linux" ] && grep -qi "microsoft" /proc/version 2>/dev/null; then \
		rm -rf "$(HOME)/Desktop/$(EXECUTABLE)"; \
		echo "WSL: Shortcut removed..."; \
	elif [ "$$OS_NAME" = "Linux" ]; then \
		rm -rf "$(HOME)/Desktop/$(EXECUTABLE)"; \
		echo "Linux: Shortcut removed..."; \
	elif echo "$$OS_NAME" | grep -qE "CYGWIN|MINGW|MSYS"; then \
		rm -f "$(USERPROFILE)/Desktop/$(EXECUTABLE).exe"; \
		echo "Cygwin/Mingw/MSYS: Shortcut removed..."; \
	elif [ "$$OS_NAME" = "Windows" ]; then \
		if [ -n "$$USERPROFILE" ]; then \
			cmd /c "if exist \"$$USERPROFILE\\Desktop\\$(EXECUTABLE).exe\" del /f /q \"$$USERPROFILE\\Desktop\\$(EXECUTABLE).exe\"" && echo "Windows: Shortcut removed..."; \
		else \
			echo "Windows: Could not determine user profile directory."; \
		fi \
	fi

# Install the project
install: build
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
		elif [ "$$OS_NAME" = "Linux" ] && grep -qi "microsoft" /proc/version 2>/dev/null; then \
			echo "WSL Detected..."; \
			cp $(EXECUTABLE) ~/Desktop/; \
		elif [ "$$OS_NAME" = "Linux" ]; then \
			echo "Linux Detected..."; \
			cp $(EXECUTABLE) ~/Desktop/; \
		elif echo "$$OS_NAME" | grep -qE "CYGWIN|MINGW|MSYS"; then \
			echo "Windows-like Environment Detected (Cygwin/MSYS)..."; \
			cp $(EXECUTABLE).exe "$$USERPROFILE/Desktop/"; \
		elif [ "$$OS_NAME" = "Windows" ]; then \
			echo "Native Windows Detected..."; \
			if [ -n "$$USERPROFILE" ]; then \
				cp $(EXECUTABLE).exe "$$USERPROFILE/Desktop/"; \
			else \
				echo "Windows: Could not determine user profile directory."; \
			fi \
		fi \
	fi
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
