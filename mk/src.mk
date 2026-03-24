SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')

ifeq ($(SOURCES),)
	$(error No source files found)
endif

OBJECTS := $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/src/%.o,$(SOURCES))
