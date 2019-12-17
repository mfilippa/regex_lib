# ------------------------------------------------------------------------------
# makefile template - MPF 12/2019
# ------------------------------------------------------------------------------

# compiler
CC = gcc

# compiler flags:
# .. all warnings
CFLAGS += -Wall
# .. generate dependency files *.d
CFLAGS += -MMD -MP
# .. #define symbols
CFLAGS += -DSYMBOL
# .. paths to search for header files
CFLAGS += -I./
# .. paths to search for source files
VPATH += ./

# object folder
OBJDIR = ./obj

# source files
SRC = main.c

# build target - linker command
debug : $(SRC:%.c=$(OBJDIR)/%.o)
	$(CC) $(CFLAGS) -o $@ $^

# compile source files
$(SRC:%.c=$(OBJDIR)/%.o) : $(SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

# auto dependencies - recompile if a header file changed
-include $(SRC:%.c=$(OBJDIR)/%.d)

# clean
.PHONY: clean 
clean :
	rm -f debug
	rm -f $(SRC:%.c=$(OBJDIR)/%.o)
	rm -f $(SRC:%.c=$(OBJDIR)/%.d)


