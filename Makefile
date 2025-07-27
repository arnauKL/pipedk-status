# Makefile for pipedk, custom stautsbar.
PROJECT = pipedk

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Installation directories
PREFIX = /usr/local
BINDIR_INSTALL = $(PREFIX)/bin

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c99 -O2
LDFLAGS =

# Find all .c files in src directory
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(PROJECT)

# Create the executable
$(PROJECT): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Install the program
install: $(PROJECT)
	install -d $(BINDIR_INSTALL)
	install -m 755 $(PROJECT) $(BINDIR_INSTALL)

# Uninstall the program
uninstall:
	rm -f $(BINDIR_INSTALL)/$(PROJECT)

# Clean build files
clean:
	rm -rf $(OBJDIR)
	rm $(PROJECT)

# Clean everything including installed files
distclean: clean uninstall

# Rebuild everything
rebuild: clean all

# Declare phony targets
.PHONY: all install uninstall clean distclean rebuild help
