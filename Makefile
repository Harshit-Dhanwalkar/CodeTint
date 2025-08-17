CC = gcc
TARGET = codetint

C_SOURCES = \
    codetint.c \
    modules/theme.c \
    modules/libcodeimage.c

TS_SOURCES = \
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
    ./tree-sitter-rust/src/parser.c \
    ./tree-sitter-rust/src/scanner.c \
    ./tree-sitter-bash/src/parser.c \
    ./tree-sitter-bash/src/scanner.c

SOURCES = $(C_SOURCES) $(TS_SOURCES)

INCLUDES = \
    -Imodules \
    -Imodules/stb \
    -I./tree-sitter/lib/include \
    -I./tree-sitter-python/src \
    -I./tree-sitter-c/src \
    -I./tree-sitter-cpp/src \
    -I./tree-sitter-javascript/src \
    -I./tree-sitter-html/src \
    -I./tree-sitter-css/src \
    -I./tree-sitter-rust/src \
    -I./tree-sitter-bash/src

LDFLAGS = \
    -lm \
    -lX11 \
    -lfontconfig \
    -lharfbuzz \
    -lpng \
    -lfreetype

CFLAGS = -Wall -Wextra -g $(INCLUDES)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
