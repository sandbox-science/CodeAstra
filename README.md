<h1 align="center">CodeAstra ~ Modern Code Editor</h1>

CodeAstra is a modern, extensible, and lightweight code editor built using C++ and Qt6, designed to offer a fast, customizable, and feature-rich development experience. Inspired by NeoVim and VSCode, it **will** provides efficient file navigation, syntax highlighting, and a powerful plugin system, making it an ideal choice for developers who need speed, flexibility, and control. With a focus on performance and usability, the editor **will** supports split views, an integrated terminal, customizable key bindings, and seamless Git integration, catering to both beginners and power users.

CodeAstra is **under active development**—stay tuned for updates or feel free to contribute and help shape its future!

## Disclaimer
This project has been tested on macOS but not yet on Windows or Linux. It should run on Linux without issues. However, on Windows, you may encounter errors. Docker will be used soon to avoid cross platform issues for development.

## How to run the project
1. Clone the project: 
``git clone https://github.com/sandbox-science/CodeAstra.git``

2. In the root directory, install the app: 
``make``

3. Run the editor:
``open CodeAstra.app``

## Windows Users
For installing the project on windows, you will need to install WSL.

```bash
# open PowerShell, then run:
wsl --install

# then, install all this packages:
sudo apt update
sudo apt install make
sudo apt install cmake
sudo apt install g++ -y
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt install qt6-base-dev
```

## Required Tools
- CMake
- Make
- g++ compiler

## Roadmap
- [x] Basic text editing
  - [x] Open a file
  - [x] Save file
  - [ ] Create a new file
- [ ] File tree navigation
- [ ] Syntax highlighting
- [ ] Plugin system

## To-Do
Find tasks to to-do on our open [issues](https://github.com/sandbox-science/CodeAstra/issues)