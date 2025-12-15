# Istanbul Metro Route Planner Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = -lm

# Target executable
TARGET = metro_planner

# Source files
SOURCES = main.c src/bst_tree.c src/graph_loader.c src/min_heap.c src/route_solver.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build successful! Run with: ./$(TARGET)"

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Install target (optional - copies to /usr/local/bin)
# WARNING: This requires sudo privileges. Review before running.
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin"
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Uninstall target
uninstall:
	@echo "Removing $(TARGET) from /usr/local/bin"
	@sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete"

# Run the program
run: $(TARGET)
	./$(TARGET)

# Rebuild (clean + build)
rebuild: clean all

# Help target
help:
	@echo "Istanbul Metro Route Planner - Makefile Help"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the project"
	@echo "  make all          - Same as 'make'"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make run          - Build and run the program"
	@echo "  make rebuild      - Clean and rebuild"
	@echo "  make install      - Install to /usr/local/bin (requires sudo)"
	@echo "  make uninstall    - Remove from /usr/local/bin (requires sudo)"
	@echo "  make help         - Show this help message"

.PHONY: all clean install uninstall run rebuild help
