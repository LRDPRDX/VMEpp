TOP_DIR :=  $(shell pwd)/../../..
INC_DIR     := $(TOP_DIR)/inc
LINKAGE     := -lVMEplusM -lVMEplus

CC          := g++
CXXFLAGS    := -std=c++11

SRC     := $(wildcard *.cpp)
TARGET  := $(patsubst %.cpp,%,$(SRC))

$(TARGET) : $(SRC)
		$(CC) -DLINUX $(CXXFLAGS) -g -I$(INC_DIR) -I$(HOME)/Tools/cereal-1.3.0/include $< $(LINKAGE) -o $@

.PHONY : clean
clean :
	rm -f $(TARGET)
