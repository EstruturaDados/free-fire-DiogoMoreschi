CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
SRC = src/FreeFire.c
BIN_DIR = bin
TARGET = $(BIN_DIR)/freefire

.PHONY: all clean run

all: $(TARGET)

$(BIN_DIR):
mkdir -p $(BIN_DIR)

$(TARGET): $(SRC) | $(BIN_DIR)
$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
./$(TARGET)

clean:
rm -rf $(BIN_DIR)
