# Makefile for the DNS changer program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = cell

# Source files
SRCS = main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link the target executable
$(TARGET): $(OBJS)
$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
rm -f $(TARGET) $(OBJS)

# Print help
help:
@echo "Usage:"
@echo "  make        - Build the program"
@echo "  make clean  - Remove build files"
@echo "  make help   - Show this help message"

.PHONY: all clean help