<p align="center">
  <img src="resources/app_icon.png" alt="CodeAstra Logo" width="200">
</p>

<div align="center">
  
  <!-- C++ CI Badge -->
  [![C++ CI](https://github.com/sandbox-science/CodeAstra/actions/workflows/cpp.yml/badge.svg)](https://github.com/sandbox-science/CodeAstra/actions/workflows/cpp.yml)
  [![Test CI](https://github.com/sandbox-science/CodeAstra/actions/workflows/test.yml/badge.svg)](https://github.com/sandbox-science/CodeAstra/actions/workflows/test.yml)

  <!-- CodeAstra Latest Release Badge -->
  <a href="https://github.com/sandbox-science/CodeAstra/releases">
    <img src="https://img.shields.io/github/v/tag/sandbox-science/CodeAstra?label=CodeAstra&color=lightblue" alt="CodeAstra Latest Release">
  </a>

  <!-- C++ Version Badge (with official C++ blue color) -->
  <a href="https://isocpp.org/">
    <img src="https://img.shields.io/badge/C%2B%2B-v20%2B-%2300599C" alt="C++ Version 20+">
  </a>

  <!-- Qt Version Badge (with official Qt green color) -->
  <a href="https://doc.qt.io/qt-6/gettingstarted.html">
    <img src="https://img.shields.io/badge/Qt-6.8-%2343B02A" alt="Qt Version 6.8">
  </a>
</div>

<h1 align="center">CodeAstra ~ Modern Code Editor</h1>

CodeAstra is a modern, extensible, and lightweight code editor built using C++ and Qt6, designed to offer a fast, customizable, and feature-rich development experience. Inspired by NeoVim and VSCode, it **will** provide efficient file navigation, syntax highlighting, and a powerful plugin system, making it an ideal choice for developers who need speed, flexibility, and control. With a focus on performance and usability, the editor **will** support split views, an integrated terminal, customizable key bindings, and seamless Git integration, catering to both beginners and power users.

> [!NOTE]
> 
> CodeAstra is **under active development**—stay tuned for updates or feel free to contribute and help shape its future!

> [!IMPORTANT]
> 
> This project has been tested on macOS, but not Windows or Linux. It should work on Linux without issues. However, you might encounter errors on Windows. To address cross-platform compatibility, Docker will be implemented soon for development.

## How to install the project
Please, check the [wiki](https://github.com/sandbox-science/CodeAstra/wiki) for recommended installation instructions.

## Required Tools
- CMake
- Make
- g++ compiler (with support of C++17 and beyond)
- Qt6 Framework

## Roadmap
- [x] Basic text editing
  - [x] Open a file
  - [x] Save file
  - [ ] Create a new file ~ in progress
- [x] File tree navigation
- [ ] Syntax highlighting ~ in progress
  - Supported Languages:
    - [x] Markdown (**foundation**)
    - [x] YAML (**foundation**)
    - [ ] C/C++ (**in progress**)
    - [ ] Golang (**in progress**)
    - [ ] Python (**Backlog**)
    - [ ] Elixir (**Backlog**)
    - more to come ... ([contribution welcomed](https://github.com/sandbox-science/CodeAstra/issues/4)) Read our [wiki](https://github.com/sandbox-science/CodeAstra/wiki/Config) for more information
- [ ] Plugin system

## To-Do
Find tasks to do on our open [issues](https://github.com/sandbox-science/CodeAstra/issues)
