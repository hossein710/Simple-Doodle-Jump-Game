# ---------------------------------------------------------------------------
# Makefile — simple-doodle-jump (Linux, SFML)
# ---------------------------------------------------------------------------

CXX      := g++

# 1. Automatically include all subdirectories inside include/
INC_DIRS := $(shell find include -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXXFLAGS := -std=c++17 -Wall -Wextra $(INC_FLAGS)
LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC_DIR  := src
OBJ_DIR  := obj
TARGET   := doodle_jump

# 2. Find all .cpp files recursively in src/ and sub-folders
SOURCES  := $(shell find $(SRC_DIR) -type f -name '*.cpp')

# 3. Mirror the src/ folder structure inside obj/
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# 4. Pattern rule that automatically creates subdirectories in obj/ if needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)