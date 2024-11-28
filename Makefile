CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -lncurses
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c) main.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/rpg_game

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR) $(OBJ_DIR)/$(SRC_DIR) $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean directories 