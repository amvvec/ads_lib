# compilation rule
$(OBJECT_DIR)/src/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@

# linkage
$(BINARIES): $(OBJECTS)
	@mkdir -p $(BINARY_DIR)
	$(COMPILER) $^ -o $@

# include dependencies
-include $(OBJECTS:.o=.d)
