include mk/config.mk
include mk/flags.mk
include mk/src.mk
include mk/rules.mk

.PHONY: all clean rebuild

all: $(BINARIES)

clean:
	rm -rf build

rebuild: clean all
