SRCS    := $(wildcard *.cpp)
OBJS    := $(patsubst %,$(OBJ_DIR)/%,$(notdir $(SRCS:.cpp=.o)))

LIB_SO  := libVMEplus.so
TARGET  := $(LIB_DIR)/$(LIB_SO)

all : $(TARGET)

$(TARGET) : $(OBJS) | $(LIB_DIR)
	$(info $nShared library compilation...)
	$(CXX) $(LDFLAGS) $^ $(LINKAGE) -o $@

$(LIB_DIR) : ; mkdir -p $@

$(OBJ_DIR)/%.o : %.cpp | $(OBJ_DIR) $(DEP_DIR)
	$(CXX) $(DEPFLAGS) -c $(CXXFLAGS) -I$(INC_DIR) $< -o $@ 

$(OBJ_DIR) : ; mkdir -p $@
$(DEP_DIR) : ; mkdir -p $@

-include $(wildcard $(DEP_DIR)/*.d)

.PHONY : install
install : $(TARGET)
	$(info $n Linking library...)
	cp $< /usr/lib && chmod 0755 /usr/lib/$(LIB_SO)
	ldconfig
	ldconfig -p | grep $(LIB_SO)
	
.PHONY : clean
clean :
	rm -rf $(OBJ_DIR) 
	rm -rf $(LIB_DIR)
	rm -rf $(DEP_DIR)
