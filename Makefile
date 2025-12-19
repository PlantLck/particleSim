CXX = g++
CXXFLAGS = -std=c++11 -O3 -march=native -Wall
LDFLAGS = -lSDL2 -lSDL2_ttf -lm

TARGET = particle_sim
SRC_DIR = src
BUILD_DIR = build
DATA_DIR = data

SOURCES = $(SRC_DIR)/main.cpp

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(DATA_DIR)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(DATA_DIR)/*.csv

run: all
	./$(BUILD_DIR)/$(TARGET)

.PHONY: all clean run directories
