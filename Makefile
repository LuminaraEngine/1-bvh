# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -mavx2 -I./headers/ -I./tests/ -g #-I../libs/include/ -L../libs/bin/

# Define the output executable and directories
TARGET = bvh
SRCDIR = src
INCLUDEDIR = headers
BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
TESTDIR = tests

# Find all cpp files in the source and test directories
SRC = $(wildcard $(SRCDIR)/*.cpp)
TESTSRC = $(wildcard $(TESTDIR)/*.cpp)

# Generate object file paths by replacing src/test with obj directory and .cpp with .o
OBJ = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))
TESTOBJ = $(patsubst $(TESTDIR)/%.cpp,$(OBJDIR)/%.o,$(TESTSRC))

# Target to compile the executable
$(BUILDDIR)/$(TARGET): $(OBJ) $(TESTOBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp files from src into .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile .cpp files from test into .o object files
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean up the build
.PHONY: clean
clean:
	rm -rf $(BUILDDIR)/$(TARGET) $(OBJDIR)
