# ---------------- Compiler and Flags ----------------
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2
INCLUDES = -Iinclude -Isrc/memory -Isrc/CRegion
LDFLAGS = 
DEFINES = -DmemType=$(MEM_TYPE)

# ---------------- Directories ----------------
SRC_DIR = src
BUILD_DIR = build
BIN = main

# ---------------- Memory Type (0 = malloc, 1 = arena, 2 = pool) ----------------
MEM_TYPE ?= 0

# ---------------- Source Files ----------------
SRC_FILES = $(shell find $(SRC_DIR) -name '*.c' ! -name 'malloc_alloc.c' ! -name 'arena_alloc.c' ! -name 'pool_malloc.c')
ifeq ($(MEM_TYPE), 0)
    SRC_FILES += $(SRC_DIR)/memory/malloc_alloc.c
endif
ifeq ($(MEM_TYPE), 1)
    SRC_FILES += $(SRC_DIR)/memory/arena_alloc.c
endif
ifeq ($(MEM_TYPE), 2)
    SRC_FILES += $(SRC_DIR)/memory/pool_malloc.c
endif

# Create .o file names
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# ---------------- Rules ----------------
all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(DEFINES) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)

print:
	@echo "Building with memType = $(MEM_TYPE)"
	@echo "Source files:"
	@printf "  %s\n" $(SRC_FILES)

.PHONY: all clean print
