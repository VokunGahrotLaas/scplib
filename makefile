DEBUG = yes

ifeq ($(DEBUG),yes)
	DEBUGFLAGS = -O0
else
	DEBUGFLAGS = -O2
endif

CC = gcc
CCFLAGS = $(DEBUGFLAGS) -Wall
LDFLAGS = 
INCLUDES = -I. -I/usr/include
LIBS = -L/usr/lib
TESTS = $(wildcard scp/tests/*.c)
TESTS_EXEC = $(filter $(wildcard *),$(subst .c,,$(subst scp/tests/,,$(TESTS))))
TESTS_EXEC := $(wildcard *.exe)

empty = 
space = $(empty) $(empty)
comma = ,

all: $(subst test_,run_test_,$(subst .c,,$(subst scp/tests/,,$(TESTS))))

run_%: %
	@./$(subst run_,,$@)

test_%: scp/tests/test_%.o
	@$(CC) -o $@ $< $(LDFLAGS) $(LIBS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDES)

.PHONY: all clear mrproper run_%

clean:
	@rm -rf $(wildcard scp/tests/*.o)

mrproper: clean
	@rm -rf $(TESTS_EXEC)
	@rm -rf $(wildcard *.exe)
