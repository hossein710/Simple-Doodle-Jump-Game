# ---------------------------------------------------------------------------
# Makefile — simple-doodle-jump (Linux, SFML)
#
# Requires SFML to be installed, e.g. on Debian/Ubuntu:
#   sudo apt-get install libsfml-dev
# ---------------------------------------------------------------------------

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR  := src
OBJ_DIR  := obj
TARGET   := doodle_jump

SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
