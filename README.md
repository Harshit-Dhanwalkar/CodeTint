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
    Navigate to the `codetint` directory (if your main source file is there) and compile `codetint.c`. You'll need to link against the Tree-sitter library and the Tree-sitter Python grammar. The exact paths might vary depending on where your submodules are built/located.

    Assuming you build the grammars within their respective submodule directories (e.g., `tree-sitter-python/src/parser.c` compiled into a static/shared library `libtree-sitter-python.a/.so`):

    ```bash
    # Example compilation command (adjust paths as needed):
    # This assumes 'tree-sitter' submodule is at the root and 'tree-sitter-python' is at the root.
    # You might need to build the grammar into a library first.

    # A more common approach is to build the grammars as shared libraries or link directly to parser.c
    # This example links directly to the parser.c files from the submodules,
    # which requires compilation of those files as part of your build.

    gcc -o codetint codetint.c \
        ./tree-sitter/lib/libtree-sitter.a \
        ./tree-sitter-python/src/parser.c \
        -I./tree-sitter/include \
        -I./tree-sitter-python/src \
        -pthread -lm # Add other necessary flags like -fPIC if creating shared libs
    ```

    - Make sure `codetint.c` is in the same directory as where you run `gcc` or adjust the path.
    - You might need to adjust the include paths (`-I`) and library paths/names (`-L`, `-l`) based on your specific system and how you compile the Tree-sitter core and grammar.
    - Often, `tree-sitter` provides a `Makefile` to build its shared library; you might need to build `libtree-sitter.so` and `libtree-sitter-python.so` separately first.

## Usage

`CodeTint` is a command-line tool designed to display syntax-highlighted code.

### Basic Usage

To highlight a Python file and display it directly in your terminal:

```bash
./codetint <your_python_file.py>
```
