#filepath: Makefile
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -O2
# INCLUDES: -Isrc allows finding <glad/glad.h> if glad.h is in src/glad/
# and <graphics/renderer.h> if renderer.h is in src/graphics/
INCLUDES = -Isrc
LDFLAGS = -lglfw -lGL -lm -ldl

SRC_BASE_DIR = src
BUILD_DIR = build
BIN_NAME = region_demo

# Source files
SRCS = $(SRC_BASE_DIR)/main.c \
       $(SRC_BASE_DIR)/graphics/renderer.c \
       $(SRC_BASE_DIR)/glad/glad.c

# Object files will be flat in BUILD_DIR:
# e.g., build/main.o, build/renderer.o, build/glad.o
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.c=.o)))

all: $(BIN_NAME)

$(BIN_NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern rule for src/main.c -> build/main.o
$(BUILD_DIR)/main.o: $(SRC_BASE_DIR)/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Pattern rule for src/graphics/renderer.c -> build/renderer.o
$(BUILD_DIR)/renderer.o: $(SRC_BASE_DIR)/graphics/renderer.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Pattern rule for src/glad/glad.c -> build/glad.o
$(BUILD_DIR)/glad.o: $(SRC_BASE_DIR)/glad/glad.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_NAME)

.PHONY: all clean