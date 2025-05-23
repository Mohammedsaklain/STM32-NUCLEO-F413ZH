# file : test.mk
# Date : 20/05/25 - 16:00

# Host Compiler
CC = gcc

# Directories
SRC_DIR       = src
SRC_INC		  = drivers
TEST_SRC_DIR  = unit_test/test_src
UNITY_DIR     = unit_test/Unity/src
TEST_INC_DIR  = unit_test/test_inc
TEST_BUILD    = unit_test/test_build

# Files
SRC_FILES  = $(wildcard $(SRC_DIR)/*.c)
TEST_FILES = $(wildcard $(TEST_SRC_DIR)/*.c)
UNITY_SRC  = $(UNITY_DIR)/unity.c
OUTPUT     = $(TEST_BUILD)/RESULT

# Flags
CFLAGS  = -Wall -Wextra -std=c99 #-DHOST_UNIT_TEST# -D is a preprocessor directive added by compiler, -D operator is same as #define macroname
INCLUDES = -I$(UNITY_DIR) -I$(SRC_INC) -I$(TEST_INC_DIR) -Iinc

# build
all:
	mkdir -p $(TEST_BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC_FILES) $(TEST_FILES) $(UNITY_SRC) -o $(OUTPUT)

run: 
	$(TEST_BUILD)/./RESULT.exe
clean:
	rm -rf $(TEST_BUILD)/RESULT.exe


.PHONY: all run clean
# TO run   - make -f test.mk
# To clean - make clean -f test.mk

# make -f test.mk tells the make to use different file for it's configuration(test.mk)