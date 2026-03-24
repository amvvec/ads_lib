# This file is for what to compile

COMPILER := gcc

# default build
BUILD ?= debug

# directories
SOURCE_DIR := src
INCLUDE_DIR := include
### TEST_DIR := tests

BUILD_DIR := build/$(BUILD)
OBJECT_DIR := $(BUILD_DIR)/obj
BINARY_DIR := $(BUILD_DIR)/bin

# binary
TARGET_EXE := launcher
### TEST_EXE := test_runner

BINARIES := $(BINARY_DIR)/$(TARGET_EXE)
### TEST_BINARY := $(BINARY_DIR)/$(TEST_EXE)
