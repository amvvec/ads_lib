STANDARD := -std=c17
INCLUDES := -I$(INCLUDE_DIR)

# dependency flags for .h files
# generate new .o files without repeat build generation
# -MMD generate dependency file (.d)
# -MP safety from deletion failure
DEPENDENCY_FLAGS := -MMD -MP

WARNINGS := \
	-Wall -Werror -Wextra -Wpedantic -Wshadow \
	-Wunused-variable \
	-Wunused-parameter

BASE_FLAGS := \
	$(STANDARD) \
	$(WARNINGS)

CFLAGS := \
	$(BASE_FLAGS) \
	$(DEPENDENCY_FLAGS)

ifeq ($(BUILD), debug)
	CFLAGS += \
		-DDEBUG \
		-O0 \
		-g3

else ifeq ($(BUILD), release)
	CFLAGS += \
		-DNDEBUG \
		-O3 \
		-g0

# default
else
	$(error Unknown BUILD=$(BUILD))
endif
