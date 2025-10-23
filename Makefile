# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=199309L
LIBS = -lncurses -lpthread -lrt

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin

# Source files
SOURCES_PEER1 = $(SRCDIR)/peer1.c
SOURCES_PEER2 = $(SRCDIR)/peer2.c

# Targets
TARGET_PEER1 = $(BINDIR)/peer1
TARGET_PEER2 = $(BINDIR)/peer2

# Default target
all: $(BINDIR) $(TARGET_PEER1) $(TARGET_PEER2)

# Create bin directory
$(BINDIR):
	mkdir -p $(BINDIR)

# Build peer1
$(TARGET_PEER1): $(SOURCES_PEER1)
	$(CC) $(CFLAGS) -o $(TARGET_PEER1) $(SOURCES_PEER1) $(LIBS)

# Build peer2
$(TARGET_PEER2): $(SOURCES_PEER2)
	$(CC) $(CFLAGS) -o $(TARGET_PEER2) $(SOURCES_PEER2) $(LIBS)

# Clean build files
clean:
	rm -rf $(BINDIR)

# Install dependencies (Ubuntu/Debian)
install-deps:
	sudo apt-get install libncurses5-dev

# Phony targets
.PHONY: all clean install-deps
