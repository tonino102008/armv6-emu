# File directories
GTEST_DIR = ./test/googletest/googletest
TEST_DIR = ./test/test-armv6-emu
TEST_INCLUDE_DIR = $(TEST_DIR)
USER_DIR = ./src
USER_INCLUDE_DIR = ./include
# Build Directory
BUILD_DIR = ./build
# C/C++ Compiler
CC = gcc
CXX = g++
# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include
CFLAGS = -Wall -Wextra
CXXFLAGS += -g -Wall -Wextra -pthread
# All tests produced by this Makefile.
TESTS = $(BUILD_DIR)/register-tests $(BUILD_DIR)/instruction-tests
# MAIN = $(BUILD_DIR)/main
# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
# Start 
all : dir $(TESTS) $(MAIN)

clean :
	@ if [ -d "$(BUILD_DIR)" ]; then \
		rm -r $(BUILD_DIR); \
	else \
		echo "The directory $(BUILD_DIR) doesn't exist."; \
	fi

dir :
	@ if [ -d "$(BUILD_DIR)" ]; then \
		echo "The directory $(BUILD_DIR) already exist."; \
	else \
		mkdir -p $(BUILD_DIR); \
	fi

$(BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(BUILD_DIR)/gtest.a : $(BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/gtest_main.a : $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/register.o : $(USER_DIR)/register.c
	$(CC) $(CFLAGS) -I$(USER_INCLUDE_DIR) -o $@ -c $(USER_DIR)/register.c

$(BUILD_DIR)/instruction.o : $(USER_DIR)/instruction.c 
	$(CC) $(CFLAGS) -I$(USER_INCLUDE_DIR) -o $@ -c $(USER_DIR)/instruction.c

$(BUILD_DIR)/register-tests.o : $(TEST_DIR)/register-tests.cc \
                    $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(USER_INCLUDE_DIR) -c $(TEST_DIR)/register-tests.cc -o $@

$(BUILD_DIR)/register-tests : $(BUILD_DIR)/register.o \
					$(BUILD_DIR)/register-tests.o $(BUILD_DIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

$(BUILD_DIR)/instruction-tests.o : $(TEST_DIR)/instruction-tests.cc \
                    $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(USER_INCLUDE_DIR) -c $(TEST_DIR)/instruction-tests.cc -o $@

$(BUILD_DIR)/instruction-tests : $(BUILD_DIR)/register.o $(BUILD_DIR)/instruction.o \
					$(BUILD_DIR)/instruction-tests.o $(BUILD_DIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# $(BUILD_DIR)/memory.o : $(USER_DIR)/memory.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/memory.h
# 	$(CC) $(CFLAGS) -c $(USER_DIR)/memory.c
# 	@ mv memory.o $@

# $(BUILD_DIR)/stack.o : $(USER_DIR)/stack.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/stack.h
# 	$(CC) $(CFLAGS) -c $(USER_DIR)/stack.c
# 	@ mv stack.o $@

# $(BUILD_DIR)/system.o : $(USER_DIR)/system.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/system.h
# 	$(CC) $(CFLAGS) -c $(USER_DIR)/system.c
# 	@ mv system.o $@

# $(BUILD_DIR)/display.o : $(USER_DIR)/display.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/display.h
# 	$(CC) $(CFLAGS) -c $(USER_DIR)/display.c
# 	@ mv display.o $@

# $(BUILD_DIR)/main.o : $(USER_DIR)/main.c $(USER_INCLUDE_DIR)/types.h $(USER_INCLUDE_DIR)/display.h \
# 					$(USER_INCLUDE_DIR)/register.h $(USER_INCLUDE_DIR)/stack.h $(USER_INCLUDE_DIR)/memory.h \
# 					$(USER_INCLUDE_DIR)/system.h
# 	$(CC) $(CFLAGS) -c $(USER_DIR)/main.c
# 	@ mv main.o $@

# $(BUILD_DIR)/main : $(BUILD_DIR)/memory.o $(BUILD_DIR)/stack.o \
# 					$(BUILD_DIR)/system.o $(BUILD_DIR)/register.o $(BUILD_DIR)/display.o \
# 					$(BUILD_DIR)/main.o
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
# End