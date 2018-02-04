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

CFLAGS = -Wall -Wextra -Wno-unused-parameter --std=c99 -fpic
CFLAGS += -g
INCLUDE = -I$(PROOT)/inc
LIBS   = -L$(LIBDIR) -l:libmem.a

SOURCES += $(shell find $(SRCDIR) -name '*.c')
SLIB = lib$(MODULE).a
DLIB = lib$(MODULE).so
BIN = $(MODULE)

BIN_OBJECT = $(OBJDIR)/$(MODULE)/main.o
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/$(MODULE)/%.o)
LIB_OBJECTS = $(filter-out $(BIN_OBJECT), $(OBJECTS))

$(OBJDIR)/$(MODULE)/%.o:$(SRCDIR)/%.c
	mkdir -p $(@D)	# generate the directory
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(@)

$(LIBDIR)/$(SLIB): $(LIB_OBJECTS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $(LIBDIR)/$(SLIB) $(LIB_OBJECTS)
	@echo "Static library built: $(LIBDIR)/$(SLIB)"

$(BINDIR)/$(BIN): $(LIBDIR)/$(LIB) $(BIN_OBJECT)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -o $(BINDIR)/$(BIN) $(BIN_OBJECT) 
	@echo "Executable application built: $(BINDIR)/$(BIN)"

all: $(LIBDIR)/$(LIB) $(BINDIR)/$(BIN)

static: $(LIB_DIR)/$(LIB)

shared: $(LIB_DIR)/$(LIB)
	$(CC) -shared -Wl,--whole-archive -o $(LIBDIR)/$(DLIB) $(LIBDIR)/$(SLIB) -Wl,--no-whole-archive
	@echo "Dynamic library built: $(LIBDIR)/$(DLIB)"

application: $(LIBDIR)/$(LIB) $(BINDIR)/$(BIN)

clean:
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)

.PHONY: all clean 
