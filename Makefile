# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -mavx2 -I./headers/ #-I../libs/include/ -L../libs/bin/

# Define the output executable and directories
TARGET = bvh
SRCDIR = src
INCLUDEDIR = headers
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

# Find all cpp files in the source directory
SRC = $(wildcard $(SRCDIR)/*.cpp)

# Generate object file paths by replacing src with obj directory and .cpp with .o
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))

# Target to compile the executable
$(BUILDDIR)/$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp files into .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up the build
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/$(TARGET) $(OBJDIR)

