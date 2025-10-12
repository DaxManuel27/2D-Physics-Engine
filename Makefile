# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -Iheaders

# Target executable
TARGET = physics_engine

# Source files
SRCS = main.cpp core/Vector2D.cpp objects/RigidBody.cpp objects/Collider.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Detect if using Homebrew on Apple Silicon or Intel Mac
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),arm64)
    # Apple Silicon (M1/M2/M3)
    INCLUDE_PATH = -I/opt/homebrew/include
    LIB_PATH = -L/opt/homebrew/lib
else
    # Intel Mac
    INCLUDE_PATH = -I/usr/local/include
    LIB_PATH = -L/usr/local/lib
endif

# Libraries
LIBS = -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIB_PATH) $(LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean run

