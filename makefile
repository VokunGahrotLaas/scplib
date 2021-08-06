CC = gcc
CCFLAGS = -std=c17 -O2 -g -Wall -Wextra -Wconversion -Werror
LDFLAGS = 
INCLUDES = -I. -I/usr/include
LIBS = -L/usr/lib
DEFINES = -DSCP_IMPLEMENTATION

TESTS_DIR = ./tests
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
	@-$(TESTS_DIR)/$<
	@echo "================================"
	@echo "Finished "$<
	@rm $(TESTS_DIR)/$<
	@echo ""

debug_%: %
	@echo "Debugging "$<"..."
	@echo "================================"
	@-gdb $(TESTS_DIR)/$<
	@echo "================================"
	@echo "Finished "$<
	@rm $(TESTS_DIR)/$<
	@echo ""

test_%: $(TESTS_DIR)/test_%.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) $(DEFINES)

strict_test_%: $(TESTS_DIR)/test_%.c
	@$(CC) -o $(TESTS_DIR)/$@ $< $(LDFLAGS) $(LIBS) $(CCFLAGS) $(INCLUDES) $(DEFINES) -Wpedantic -DSCP_PEDANTIC

clean:

mrproper: clean
	@rm -rf $(TESTS_EXEC) $(STRICT_TESTS_EXEC)

install:
	@cp -r ./scp /usr/include/

.PHONY: all run_tests run_strict_tests run_% debug_% test_% strict_test_% clear mrproper install