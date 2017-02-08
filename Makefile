TARGET=ljmd.x

CC = gcc
LINKER   = gcc -o

CFLAGS =-Wall -I. -g -O3 -ffast-math -fomit-frame-pointer
LFLAGS   = -lm

SRCDIR   = ./src
OBJDIR   = ./obj
BINDIR   = ./bin
INCDIR   = ./include

CFLAGS += -I$(INCDIR) -I$(SRCDIR)

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJ  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


$(BINDIR)/$(TARGET): $(OBJ)
	@$(LINKER) $@ $(LFLAGS) $(OBJ) -Wl,-rpath,$(OBJDIR)/.
	@echo "Linking complete!"

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@rm -f $(OBJ)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@rm -f $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
