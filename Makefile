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

BIN_SOURCES += $(shell find $(SRCDIR)/app -name '*.c')
LIB_SOURCES += $(shell find $(SRCDIR)/lib -name '*.c')
SLIB = lib$(MODULE).a
DLIB = lib$(MODULE).so
BIN = $(MODULE)

BIN_OBJECTS = $(BIN_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/$(MODULE)/%.o)
LIB_OBJECTS = $(LIB_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/$(MODULE)/%.o)

$(OBJDIR)/$(MODULE)/%.o:$(SRCDIR)/%.c
	mkdir -p $(@D)	# generate the directory
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $(@)

$(LIBDIR)/$(SLIB): $(LIB_OBJECTS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $(LIBDIR)/$(SLIB) $(LIB_OBJECTS)
	@echo "Static library built: $(LIBDIR)/$(SLIB)"

$(BINDIR)/$(BIN): $(LIBDIR)/$(LIB) $(BIN_OBJECTS)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -o $(BINDIR)/$(BIN) $(BIN_OBJECTS) 
	@echo "Executable application built: $(BINDIR)/$(BIN)"

all: $(BINDIR)/$(BIN)

static: $(LIB_DIR)/$(LIB)

shared: $(LIB_DIR)/$(LIB)
	$(CC) -shared -Wl,--whole-archive -o $(LIBDIR)/$(DLIB) $(LIBDIR)/$(SLIB) -Wl,--no-whole-archive
	@echo "Dynamic library built: $(LIBDIR)/$(DLIB)"

app: $(LIBDIR)/$(LIB) $(BINDIR)/$(BIN)

clean:
	rm -rf $(BINDIR)
	rm -rf $(OBJDIR)
	rm -rf $(LIBDIR)

.PHONY: all clean 
