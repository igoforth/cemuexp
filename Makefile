# -----------------------------------------
# A wrapper for llvm ia32 instruction decoding
# Author: Ian Goforth
# 
# A 3-second Makefile
# -----------------------------------------

TARGET	= decoder
CC	= gcc
AR	= ar rcs

SRCDIR	= src
LIBDIR	= lib
OBJDIR	= obj
BINDIR	= bin

SRCS	:= $(wildcard $(SRCDIR)/*.c)
LIBS	:= $(wildcard $(LIBDIR)/$(LIBRARY)/*.c)
SOBJS	:= $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
LOBJS	:= $(LIBS:$(LIBDIR)/%.c=$(OBJDIR)/%.o)
rm	= rm -rf

all: $(BINDIR)/$(LIBRARY) $(BINDIR)/$(TARGET)

$(BINDIR)/$(LIBRARY): $(LOBJS)
	@mkdir -p $(@D)
	@$(AR) $@.a $<

$(LOBJS): $(OBJDIR)/%.o: $(LIBDIR)/%.c
	@mkdir -p $(@D)
	@$(CC) -c $< -o $@

$(BINDIR)/$(TARGET): $(SOBJS)
	@mkdir -p $(@D)
	@$(CC) -o $@ $(SOBJS)

$(SOBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	@$(CC) -l$(LIBRARY) -L$(LIBDIR)/$(LIBRARY) -c $< -o $@

.PHONY: clean
clean:
	@$(rm) $(OBJDIR)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)