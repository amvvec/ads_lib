CC		:= gcc
CFLAGS +=	-fsanitize=address,undefined,leak \
			-fsanitize-address-use-after-scope \
			-Wcast-qual \
			-Wcast-align=strict \
			-Wstrict-aliasing=2 \
			-Wdouble-promotion \
			-O0 -fno-omit-frame-pointer \
			-Wduplicated-cond \
			-Wduplicated-branches \
			-Wlogical-op \
			-Wjump-misses-init \
			-Wpacked \
			-Wredundant-decls \
			--param=ssp-buffer-size=4 \
			-std=c17 \
			-Wall \
			-Wextra \
			-Wpedantic \
			-Werror \
			-Wshadow \
			-Wconversion \
			-Wsign-conversion \
			-Wformat=2 \
			-Wformat-signedness \
			-Wformat-security \
			-Wnull-dereference \
			-Wswitch-enum \
			-Wswitch-default \
			-Wfloat-equal \
			-Wundef \
			-Wimplicit-fallthrough \
			-Wunused-parameter \
			-Wunused-variable \
			-Wparentheses \
			-Wmisleading-indentation \
			-Wstrict-prototypes \
			-Wold-style-definition \
			-fno-common \
			-fstack-protector-strong \
			-D_FORTIFY_SOURCE=2 \
			-g

SRC_DIR 	:= src
BUILD_DIR 	:= build

EXEC 		:= launcher

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
HEADERS := $(shell find $(SRC_DIR) -name '*.h')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(BUILD_DIR) $(EXEC)

$(BUILD_DIR):
	@mkdir -p $@

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(EXEC)

.PHONY: all clean
