TARGET 	:= dump 
LINKAGE += -lCAENDigitizer

$(TARGET) : dump.cpp 
	$(CXX) $(CXXFLAGS) $(LINKAGE) -I$(INC_DIR) $< -o $@

.PHONY : clean
clean :
	rm -rf $(TARGET)
