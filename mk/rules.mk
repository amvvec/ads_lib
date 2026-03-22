# linkage
$(BINARY): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(COMPILER) $^ -o $@

# compilation
$(OBJECT_DIR)/src/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@

# dependency
-include $(OBJECTS:.o=.d)
