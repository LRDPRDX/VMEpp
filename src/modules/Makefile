MODULE_SRCS 	:= $(wildcard *.cpp)
MODULE_HDRS 	:= $(wildcard $(INC_DIR)/modules/*.h)
MODULE_OBJ_DIR 	:= $(OBJ_DIR)/modules

MODULE_OBJS	:= $(patsubst %,$(MODULE_OBJ_DIR)/%,$(notdir $(MODULE_SRCS:.cpp=.o)))

LIB_SO 		:= libVMEplusM.so 
TARGET      := $(LIB_DIR)/$(LIB_SO)

all : directories $(TARGET)

.PHONY : directories
directories : $(MODULE_OBJ_DIR)

$(MODULE_OBJ_DIR) :
	$(MKDIR_P) $@

vpath %.h $(INC_DIR)/modules

$(MODULE_OBJ_DIR)/%.o : %.cpp $(HDRS) %.h
	$(CC) -c $(CXXFLAGS) -I$(INC_DIR) $< -o $@ 

$(TARGET) : $(MODULE_OBJS)
	$(info $nShared library compilation...)
	$(CC) $(LDFLAGS) $^ $(LINKAGE) -o $@

.PHONY : install
install : $(TARGET)
	$(info $n Linking library...)
	cp $< /usr/lib && chmod 0755 /usr/lib/$(LIB_SO)
	ldconfig
	ldconfig -p | grep $(LIB_SO)

.PHONY : clean
clean :
	rm -f $(MODULE_OBJS)
	rm -f $(TARGET)
