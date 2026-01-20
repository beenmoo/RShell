CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I RShell/Code
LDFLAGS :=

SRC := $(shell find RShell/Code -name '*.cpp')
OBJ := $(SRC:.cpp=.o)
BIN_DIR := bin
TARGET := $(BIN_DIR)/rshell

.PHONY: all run clean

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(BIN_DIR) $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	find RShell/Code -name '*.o' -delete
	rm -f $(TARGET)
