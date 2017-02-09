TARGET    = ljmd.x
SRCDIR    = src
OBJDIR    = obj
BINDIR    = bin
INCDIR    = include
SERIALDIR = serial


CC        = gcc -pg
LINKER    = gcc -pg -o


CFLAGS    = -Wall -I. -I$(INCDIR) -I$(SRCDIR) -g -O3 -ffast-math #-fomit-frame-pointer
SCFLAGS   = -Wall -I. -g -O3 -ffast-math #-fomit-frame-pointer
LFLAGS    = -lm


SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJ      := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)


# rules for makeing parallel version of ljmd.
$(BINDIR)/$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	@$(LINKER) $@ $(LFLAGS) $(OBJ) -Wl,-rpath,$(OBJDIR)/.
	@echo "Linking complete!"


$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"


# rules for makeing serial version of ljmd.
serial: $(BINDIR)/ljmd-serial.x


$(BINDIR)/ljmd-serial.x: $(SERIALDIR)/ljmd_serial.o
	@mkdir -p $(dir $@)
	@$(LINKER) $@ $(SCFLAGS) $^ $(LFLAGS) -Wl,-rpath,./$(SERIALDIR)/.
	@echo "Linking complete!"


$(SERIALDIR)/ljmd_serial.o: $(SERIALDIR)/ljmd.c
	@mkdir -p $(dir $@)
	@$(CC) -c $(SCFLAGS) $< -o $@
	@echo "Compiled "$<" successfully!"


# rules for cleaning up
.PHONY: clean
clean:
	@rm -f $(OBJ) $(SERIALDIR)/*.o
	@rm -rf $(OBJDIR)
	@echo "Cleanup complete!"


.PHONY: remove
remove: clean
	@rm -f $(BINDIR)/$(TARGET)
	@rm -rf $(BINDIR)
	@echo "Executable removed!"
