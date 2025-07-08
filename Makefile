ifeq ($(OS),Windows_NT)
	OUTPUT_CMD = chess.exe
	CREATE_DIR = @if not exist $(OBJDIR) mkdir $(OBJDIR)
	CLEAN_CMD = del /q $(OBJDIR)\*.o $(OUTPUT) *.ilk *.pdb
	REMOVE_DIR = @if exist $(OBJDIR) if exist $(OBJDIR)\nul if not exist "$(OBJDIR)\*" rmdir $(OBJDIR)
else
	OUTPUT_CMD = chess
	CREATE_DIR = mkdir -p $(OBJDIR)
	CLEAN_CMD = find $(OBJDIR) -type f -name '*.o' -delete; rm -f $(OUTPUT) *.ilk *.pdb
	REMOVE_DIR = [ -d $(OBJDIR) ] && [ -z "$$(ls -A $(OBJDIR))" ] && rmdir $(OBJDIR)
endif

# Compiler
CC = clang++
# CC = g++


# Folders
INCLUDEDIR = include
SRCDIR = src
OBJDIR = build

# Compiler flags
CPPFLAGS = -g -Wall -std=c++23 -I$(INCLUDEDIR)

# Name of the output binary
OUTPUT = $(OUTPUT_CMD)

# List of source files
SOURCES = $(SRCDIR)/main.cpp $(SRCDIR)/AlfaBeta.cpp $(SRCDIR)/Board.cpp $(SRCDIR)/Game.cpp $(SRCDIR)/Piece.cpp $(SRCDIR)/Types.cpp

# List of object files
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(OUTPUT)

# Linking final executable
$(OUTPUT): $(OBJECTS)
# $@ = target name (OUTPUT), $^ = all prerequisites (OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .cpp source files into .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
# Create object directory if it doesn't exist
	$(CREATE_DIR)
# Compile source file ($<) into object file ($@)
	$(CC) $(CPPFLAGS) -c $< -o $@

# Remove generated files
clean:
	$(CLEAN_CMD)
	$(REMOVE_DIR)