OS := $(shell uname)
ifeq ($(OS), Windows_NT)
	COMPILER := cl.exe
	RELEASE_ARGS := /std:c++latest /O2 /DNDEBUG /EHsc
	OUTPUT_DIR := ./bin
	RELEASE_OUTPUT := $(OUTPUT_DIR)/horizon.exe
	RUN_CMD := $(OUTPUT_DIR)/horizon.exe ./sample/expr.hr
else
	COMPILER := g++
	RELEASE_ARGS := -std=c++23 -O3 -DNDEBUG -march=native -mtune=native -masm=intel
	DEBUG_ARGS := -std=c++23 -g -pg -ggdb3 -Wall -Wextra -Wuninitialized -Wstrict-aliasing -Wshadow -pedantic \
				  -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wunused
	OUTPUT_DIR := ./bin
	RELEASE_OUTPUT := $(OUTPUT_DIR)/horizon
	DEBUG_OUTPUT := $(OUTPUT_DIR)/horizon_debug
	RUN_CMD := $(DEBUG_OUTPUT) ./sample/expr.hr
endif

SOURCES := \
	./deps/string/string.cc \
	./src/misc/misc.cc \
	./src/errors/errors.cc \
	./src/lexer/lexer.cc \
	./src/colorize/colorize.cc \
	./src/parser/parser.cc \
	./src/entry/horizon.cc \
	./src/defines/keywords_primary_data_types.cc

.PHONY: all release debug clean run strip

all: release debug
ifeq ($(OS), Windows_NT)
	@echo "Skipping strip step on Windows"
else
	strip $(RELEASE_OUTPUT)
endif

release:
	@echo "Building release version..."
	@mkdir -p $(OUTPUT_DIR)
ifeq ($(OS), Windows_NT)
	$(COMPILER) $(RELEASE_ARGS) $(SOURCES) /Fo$(OUTPUT_DIR)/ /Fe$(RELEASE_OUTPUT)
else
	$(COMPILER) $(RELEASE_ARGS) $(SOURCES) -o $(RELEASE_OUTPUT)
endif

debug:
ifeq ($(OS), Windows_NT)
	@echo "Skipping debug build on Windows"
else
	@echo "Building debug version..."
	@mkdir -p $(OUTPUT_DIR)
	$(COMPILER) $(DEBUG_ARGS) $(SOURCES) -o $(DEBUG_OUTPUT)
endif

strip:
	@echo "Stripping release binary..."
	strip $(RELEASE_OUTPUT)

clean:
	@echo "Cleaning up..."
	@rm -rf $(OUTPUT_DIR)

run:
	@echo "Running..."
	$(RUN_CMD)
