CC = gcc
CFLAGS = -std=c23 -pedantic -g -ggdb -Wswitch -Werror=switch
LDFLAGS = -Iinclude -Llib -lraylib -lgdi32 -lwinmm

BUILD_DIR = build/win
TARGET = $(BUILD_DIR)/main.exe
SRC = src/*.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf build