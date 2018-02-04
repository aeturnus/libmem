PROOT = $(shell pwd)
BINDIR = $(PROOT)/bin
OBJDIR = $(PROOT)/obj
LIBDIR = $(PROOT)/lib
SRCDIR = $(PROOT)/src

MODULE = mem

CC = gcc
CXX = g++
LD = ld
AR = ar

CFLAGS = -Wall -Wextra -Wno-unused-parameter --std=c99
CFLAGS += -g
INCLUDE = -I$(PROOT)/inc
LIBS   = -L$(LIBDIR) -lmem

SOURCES += $(shell find $(SRCDIR) -name '*.c')
LIB = lib$(MODULE).a
BIN = $(MODULE)

BIN_OBJECT = $(OBJDIR)/$(MODULE)/main.o
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/$(MODULE)/%.o)
LIB_OBJECTS = $(filter-out $(BIN_OBJECT), $(OBJECTS))

$(OBJDIR)/$(MODULE)/%.o:$(SRCDIR)/%.c
	mkdir -p $(@D)	# generate the directory
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(@)

$(LIBDIR)/$(LIB): $(LIB_OBJECTS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $(LIBDIR)/$(LIB) $(LIB_OBJECTS)
	@echo "Library built"

$(BINDIR)/$(BIN): $(LIBDIR)/$(LIB) $(BIN_OBJECT)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -o $(BINDIR)/$(BIN) $(BIN_OBJECT) 
	@echo "Executable driver built"

all: $(LIBDIR)/$(LIB) $(BINDIR)/$(BIN)

driver: $(LIBDIR)/$(LIB) $(BINDIR)/$(BIN)

clean:
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)

.PHONY: all clean 
