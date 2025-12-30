CC = clang
CFLAGS = -Wall -Wextra -g
LIBS = -lncurses

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/limeos-installation-wizard

SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES = $(shell find $(SRCDIR) -type d -exec printf "-I{} " \;)
CFLAGS += $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
