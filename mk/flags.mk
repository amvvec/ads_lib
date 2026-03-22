# This file is for how to compile

STANDARD := -std=c17

INCLUDES := -I$(INCLUDE_DIR)

# Dependency flags for .h files
# generates new .o files without repeat build
# -MMD generate dependency file (.d)
# -MP safety from deletion failure
DEPENDENCY_FLAGS := \
	-MMD \
	-MP

DEBUG_FLAGS := \
	-DDEBUG \
	-O0 \
	-g3 \
	-fno-omit-frame-pointer

WARNINGS := \
	-Wall \
	-Werror \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wunused-parameter -Wunused-variable

SANITIZERS := \
    -fsanitize=address \
	-fsanitize=undefined \
	-fsanitize=leak

# Security and hardening
SECURITY := \
	-fstack-protector-strong \
	-D_FORTIFY_SOURCE=2 \
    -fno-common

BASE_FLAGS := \
	$(STANDARD) \
	$(WARNINGS)

CFLAGS := \
	$(BASE_FLAGS) \
	$(DEPENDENCY_FLAGS)

# configuration

ifeq ($(BUILD), debug)
	CFLAGS += \
		$(DEBUG_FLAGS)

else ifeq ($(BUILD), release)
	CFLAGS += \
		-DNDEBUG \
		-O3

else ifeq ($(BUILD), sanitize)
	CFLAGS += \
		$(SANITIZERS)

else ifeq ($(BUILD), security)
	CFLAGS += \
		$(SECURITY)

else
	$(error Unknown BUILD=$(BUILD))
endif
