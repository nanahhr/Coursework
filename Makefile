# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Target executable name
TARGET = task13

# For deleting the executable on clean
TARGET_DEL = $(TARGET).exe

# Source files
SRCS = task13.cpp grid.cpp ui.cpp solver.cpp display.cpp input.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule: build the executable
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)
