TARGET    = ljmd.x
SRCDIR    = src
OBJDIR    = obj
BINDIR    = bin
INCDIR    = include
SERIALDIR = serial
TESTDIR   = tests

CC        = gcc -pg
LINKER    = gcc -pg -o


CFLAGS    = -Wall -I. -I$(INCDIR) -I$(SRCDIR) -g -O3 -ffast-math #-fomit-frame-pointer
SCFLAGS   = -Wall -I. -g -O3 -ffast-math #-fomit-frame-pointer
LFLAGS    = -lm


SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJ      := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
MOBJ      := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.mo)
LIB      := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.so)
MAINOBJ  := $(OBJDIR)/main.o
OBJXMAIN := $(filter-out $(MAINOBJ),$(OBJ))

# rules for makeing parallel version of ljmd.
$(BINDIR)/$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	@$(LINKER) $@ $(LFLAGS) $(OBJ) -Wl,-rpath,$(OBJDIR)/.
	@echo "Linking complete!"


$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

morse: $(BINDIR)/morsemd.x

$(BINDIR)/morsemd.x: $(MOBJ)
	@mkdir -p $(dir $@)
	@$(LINKER) $@ $(LFLAGS) $(MOBJ) -Wl,-rpath,$(OBJDIR)/.
	@echo "Linking complete!"


$(MOBJ): $(OBJDIR)/%.mo : $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -D__MORSE__
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


tests: $(TESTDIR)/ljmd-tests.x

$(TESTDIR)/ljmd-tests.x: $(OBJXMAIN) $(TESTDIR)/ljmd-tests.o
	@mkdir -p $(dir $@)
	@$(LINKER) $@ $(LFLAGS) $(OBJXMAIN) $(TESTDIR)/ljmd-tests.o  -Wl,-rpath,$(OBJDIR)/.
	@echo "Linking complete!"

$(TESTDIR)/ljmd-tests.o: $(TESTDIR)/tests.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -D __TESTS__
	@echo "Compiled "$<" successfully!"

shared: $(OBJDIR)/ljmd.so

$(OBJDIR)/ljmd.so:  
	@mkdir -p $(dir $@)
	@gcc -shared -o $@ $(OBJ)
	@echo "Compiled shared objects!"

# rules for cleaning up
.PHONY: clean
clean:
	@rm -f $(OBJ) $(SERIALDIR)/*.o $(TESTDIR)/*.o
	@rm -rf $(OBJDIR)
	@echo "Cleanup complete!"


.PHONY: remove
remove: clean
	@rm -f $(BINDIR)/$(TARGET) $(TESTDIR)/*.x
	@rm -rf $(BINDIR)
	@echo "Executable removed!"
