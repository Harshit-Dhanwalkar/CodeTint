# CodeTint

## Introduction

`CodeTint` is a lightweight and powerful command-line utility designed for fast and accurate syntax highlighting of source code. Built entirely in C and leveraging the robust parsing capabilities of **Tree-sitter**, `CodeTint` provides highly detailed and precise highlighting. Currently, it offers comprehensive support for **Python** code, transforming plain text into visually appealing, color-coded output.

Functioning much like the classic `cat` command, `CodeTint` reads your source files and outputs their content, but with the added benefit of beautiful syntax highlighting. You can choose to display the highlighted code directly in your terminal using **ANSI escape codes**, or generate **HTML** output for web integration or browser viewing. `CodeTint` aims to provide a quick, efficient, and visually enhanced way to view your code.

## Installation

To get `CodeTint` up and running, follow these steps:

### Prerequisites

- **C Compiler:** You'll need a C compiler like GCC or Clang.
- **Git:** To clone the repository and its submodules.
- **Tree-sitter Library:** `CodeTint` depends on the core Tree-sitter library.
- **Tree-sitter Python Grammar:** The specific grammar for parsing Python code.

### Build Steps

1.  **Clone the Repository:**
    Start by cloning the `CodeTint` repository from GitHub:

    ```bash
    git clone [https://github.com/your-username/CodeTint.git](https://github.com/your-username/CodeTint.git)
    cd CodeTint
    ```

    (Remember to replace `your-username/CodeTint.git` with your actual repository URL.)

2.  **Initialize Submodules:**
    `CodeTint` uses Git submodules for `tree-sitter` and `tree-sitter-python`. Initialize and update them:

    ```bash
    git submodule update --init --recursive
    ```

    This will download the necessary Tree-sitter core library and the Python grammar.

3.  **Compile the Application:**
    Navigate to your project's root directory. Use the following `gcc` command to compile `CodeTint`. This command includes common warning flags (`-Wall`, `-Wextra`), debugging information (`-g`), and directly links the necessary Tree-sitter source files and grammar:

    ```bash
    gcc -Wall -Wextra -g \
        -Imodules \
        -I./tree-sitter/lib/include \
        -I./tree-sitter-python/src \
        -I./tree-sitter-c/src \
        -I./tree-sitter-cpp/src \
        -I./tree-sitter-javascript/src \
        -I./tree-sitter-html/src \
        -I./tree-sitter-css/src \
        codetint.c modules/theme.c \
        ./tree-sitter/lib/src/lib.c \
        ./tree-sitter-python/src/parser.c \
        ./tree-sitter-python/src/scanner.c \
        ./tree-sitter-c/src/parser.c \
        ./tree-sitter-cpp/src/parser.c \
        ./tree-sitter-cpp/src/scanner.c \
        ./tree-sitter-javascript/src/parser.c \
        ./tree-sitter-javascript/src/scanner.c \
        ./tree-sitter-html/src/parser.c \
        ./tree-sitter-html/src/scanner.c \
        ./tree-sitter-css/src/parser.c \
        ./tree-sitter-css/src/scanner.c \
        -o codetint
    ```

    - This command assumes `codetint.c` is in the same directory where you run `gcc`, and that `tree-sitter` and `tree-sitter-python` are submodules cloned into the root of your project.
    - It directly compiles the Tree-sitter core's `lib.c` and the Python grammar's `parser.c` and `scanner.c` alongside your main `codetint.c`.

## Usage

`CodeTint` is a command-line tool designed to display syntax-highlighted code.

### Basic Usage

To highlight a Python file and display it directly in your terminal:

```bash
./codetint <your_python_file.py>
```

## Specifying a Color Theme

You can choose from several built-in color themes using the -c option:

```bash
./codetint -c tokyonight-night examples/test1.py
```

Available themes: default, `gruvbox`, `tokyonight-night`, `tokyonight-storm`, `catppuccin-mocha`, `dracula`, `nord`, `solarized-dark`, `solarized-light`, `one-dark`, `monokai`, `github-dark`

## Outputting to HTML

To generate HTML output for embedding in web pages or viewing in a browser, use the `--html` flag and redirect the output to an HTML file:

```bash
./codetint -q custom-highlights.scm your_code.py
```

Example: Highlighting a Python file with a specific theme

```bash
./codetint -c dracula my_script.py
```

# TODO

Here are some features and improvements planned for CodeTint:

- [x] Add highlighting for other languages: Extend support beyond Python to highlight a variety of programming and scripting languages.
  - [x] python
  - [ ] Rust
  - [x] C
  - [x] C++
  - [x] Javascript
  - [x] html
  - [x] css
  - [ ] bash
  - [ ] lua
- [x] Add line numbers: Implement an option to display line numbers alongside the highlighted code.
- [x] Optimize code by separating themes: Move theme definitions from main.c into separate files or a more modular structure for easier management and extensibility.
- [ ] Support for incremental parsing (Live Update): Extend this tool to watch files for changes and update highlighting live. This would involve using ts_parser_parse and re-parsing only changed parts for efficiency.
- [ ] Support for piping input: Allow CodeTint to read code directly from standard input (stdin), enabling use in pipelines (e.g., cat file.py | ./codetint).
- [ ] External theme configuration: Implement a mechanism to load themes from external configuration files (e.g., JSON, YAML) without recompilation.
- [ ] Configuration file support: Add a configuration file (e.g., .codetintrc) for default settings, such as preferred theme or default language.
- [ ] More robust error handling: Improve error messages and handling for file operations, parsing, and invalid arguments.
- [ ] Support for different line ending styles: Ensure correct rendering across various operating systems (Windows, Linux, macOS).
- [ ] Packaging and Distribution: Explore options for easier installation via package managers.
