MODULE_SRCS 	:= $(wildcard *.cpp)
MODULE_OBJ_DIR 	:= $(OBJ_DIR)/modules

MODULE_OBJS	:= $(patsubst %,$(MODULE_OBJ_DIR)/%,$(notdir $(MODULE_SRCS:.cpp=.o)))

LIB_SO 		:= libVMEplusM.so
TARGET      := $(LIB_DIR)/$(LIB_SO)

all : $(TARGET)

$(MODULE_OBJ_DIR)/%.o : %.cpp | $(MODULE_OBJ_DIR)
	$(CXX) $(DEPFLAGS) -c $(CXXFLAGS) -I$(INC_DIR) $< -o $@ 

$(MODULE_OBJ_DIR) : ; mkdir -p $@

$(TARGET) : $(MODULE_OBJS) | $(LIB_DIR)
	$(info $nShared library compilation...)
	$(CXX) $(LDFLAGS) $^ $(LINKAGE) -o $@

$(LIB_DIR) : ; mkdir -p $@

-include $(wildcard $(DEP_DIR)/*.d)

.PHONY : install
install : $(TARGET)
	$(info $n Linking library...)
	cp $< /usr/lib && chmod 0755 /usr/lib/$(LIB_SO)
	ldconfig
	ldconfig -p | grep $(LIB_SO)


.PHONY : clean
clean :
	rm -rf $(MODULE_OBJ_DIR) 
	rm -f $(TARGET)
