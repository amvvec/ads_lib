include mk/config.mk
include mk/flags.mk
include mk/src.mk
include mk/rules.mk

.PHONY: all clean print print-config

all: $(BINARY)

clean:
	rm -rf $(BUILD_DIR)
