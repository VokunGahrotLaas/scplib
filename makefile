DEBUG = no

ifeq ($(DEBUG),yes)
	DEBUGFLAGS = -O0
else
	DEBUGFLAGS = -O2
endif

CC = gcc
CCFLAGS = $(DEBUGFLAGS) -std=gnu2x -Wall -Wextra -Wconversion -Werror
LDFLAGS = 
INCLUDES = -I. -I/usr/include
LIBS = -L/usr/lib
TESTS = $(wildcard tests/*.c)
TESTS_EXEC = $(subst .c,,$(TESTS))
STRICT_TESTS_EXEC = $(addprefix tests/strict_,$(subst tests/,,$(TESTS_EXEC)))

empty = 
space = $(empty) $(empty)
tab = $(empty)	$(empty)
comma = ,
define newline

$(empty)
endef

all: tests

tests: $(subst test_,run_test_,$(subst .c,,$(subst tests/,,$(TESTS))))

run_%: %
	@echo "Running "$<"..."
	@echo "================================"
	@./tests/$<
	@echo "================================"
	@echo "Finished "$<
	@echo ""

test_%: tests/test_%.c
	@$(CC) -o tests/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES)

strict_test_%: tests/test_%.c
	@$(CC) -o tests/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) -Wpedantic $(INCLUDES)

clean:

mrproper: clean
	@rm -rf $(TESTS_EXEC) $(STRICT_TESTS_EXEC)

install:
	@cp -r ./scp /usr/include/

.PHONY: all clear mrproper run_% test_% strict_test_% tests install