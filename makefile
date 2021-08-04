DEBUG = no

ifeq ($(DEBUG),yes)
	DEBUGFLAGS = -O0
else
	DEBUGFLAGS = -O2
endif

STDC90 = c90 -ansi
STDC95 = iso9899:199409
STDC99 = c99
STDC11 = c11
STDC17 = c17
STDC23 = c2x

CC = gcc
CCFLAGS = $(DEBUGFLAGS) -std=$(STDC99)  -Wall -Wextra -Wconversion -Werror
LDFLAGS = 
INCLUDES = -I. -I/usr/include
LIBS = -L/usr/lib
TESTS_DIR = tests
TESTS = $(wildcard $(TESTS_DIR)/*.c)
TESTS_EXEC = $(subst .c,,$(TESTS))
STRICT_TESTS_EXEC = $(addprefix $(TESTS_DIR)/strict_,$(notdir $(TESTS_EXEC)))

empty = 
space = $(empty) $(empty)
tab = $(empty)	$(empty)
comma = ,
define newline

$(empty)
endef

all: tests

run_tests: $(subst test_,run_test_,$(subst .c,,$(subst $(TESTS_DIR)/,,$(TESTS))))

run_strict_tests: $(subst test_,run_strict_test_,$(subst .c,,$(subst $(TESTS_DIR)/,,$(TESTS))))

run_%: %
	@echo "Running "$<"..."
	@echo "================================"
	@./tests/$<
	@echo "================================"
	@echo "Finished "$<
	@rm $(TESTS_DIR)/$<
	@echo ""

test_%: $(TESTS_DIR)/test_%.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES)

strict_test_%: $(TESTS_DIR)/test_%.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) -Wpedantic -DSCP_PEDANTIC

test_shell: $(TESTS_DIR)/test_shell.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) -lncurses

strict_test_shell: $(TESTS_DIR)/test_shell.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) -lncurses -Wpedantic -DSCP_PEDANTIC

clean:

mrproper: clean
	@rm -rf $(TESTS_EXEC) $(STRICT_TESTS_EXEC)

install:
	@cp -r ./scp /usr/include/

.PHONY: all run_tests run_strict_tests run_% test_% strict_test_% test_shell strict_test_shell clear mrproper install