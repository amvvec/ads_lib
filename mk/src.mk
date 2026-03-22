SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')

OBJECTS := $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/src/%.o,$(SOURCES))
