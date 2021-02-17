SRCS 	:= $(wildcard *.cpp) 
OBJS	:= $(patsubst %,$(OBJ_DIR)/%,$(notdir $(SRCS:.cpp=.o)))

LIB_SO  := libVMEplus.so
TARGET  := $(LIB_DIR)/$(LIB_SO)

all : directories $(TARGET)

.PHONY : directories
directories : $(OBJ_DIR)

$(OBJ_DIR) :
	$(MKDIR_P) $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.cpp $(HDRS)
	$(CC) -c $(CXXFLAGS) -I$(INC_DIR) $< -o $@ 

$(TARGET) : $(OBJS)
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
	rm -f $(OBJS)
	rm -f $(TARGET)
