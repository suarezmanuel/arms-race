CC = gcc
CFLAGS = -std=c99 -pedantic -g -ggdb -IC:/raylib/include
LDFLAGS = -Lbuild/win -lraylib -lgdi32 -lwinmm

BUILD_DIR = build/win
TARGET = $(BUILD_DIR)/main.exe
SRC = src/*.c
RAYLIB_DLL = C:/raylib/lib/raylib.dll

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) $(BUILD_DIR)/raylib.dll
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR)/raylib.dll: $(RAYLIB_DLL) | $(BUILD_DIR)
	cp "$<" "$@"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf build