CC = gcc
CFLAGS = -rdynamic -Wall -Wextra -g -I/usr/include/postgresql `pkg-config --cflags gtk+-3.0`
LDFLAGS = -rdynamic -lpq `pkg-config --libs gtk+-3.0`
TARGET = prog

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
SRC_DIR = ./src
LIB_DIR = ./lib

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/login.c $(SRC_DIR)/register.c $(LIB_DIR)/windows_utils.c $(LIB_DIR)/postgres_client.c
OBJS = $(BIN_DIR)/main.o $(BIN_DIR)/login.o $(BIN_DIR)/register.o $(BIN_DIR)/windows_utils.o $(BIN_DIR)/postgres_client.o

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$$(BIN_DIR)/main.o: $(SRC_DIR)/main.c | $(BIN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/register.o: $(SRC_DIR)/register.c | $(BIN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/windows_utils.o: $(LIB_DIR)/windows_utils.c | $(BIN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BIN_DIR)/postgres_client.o: $(LIB_DIR)/postgres_client.c | $(BIN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)
	
$(BIN_DIR)/login.o:  $(SRC_DIR)/login.c | $(BIN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
