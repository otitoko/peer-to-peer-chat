CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lsodium

#Folders
SRC_DIR=src
BUILD_DIR=build
INCLUDE_DIR=include

#Files
SOURCES=$(SRC_DIR)/main.c $(SRC_DIR)/crypto.c
OBJECTS=$(BUILD_DIR)/main.o $(BUILD_DIR)/crypto.o
TARGET=p2pchat

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
