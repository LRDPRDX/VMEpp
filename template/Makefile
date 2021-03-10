TOP_DIR := /home/sayka/course/VMEpp
INC_DIR     := $(TOP_DIR)/inc
LINKAGE     := -lVMEplusM -lVMEplus

CC          := g++
CXXFLAGS    := -std=c++11

SRC     := $(wildcard *.cpp)
TARGET  := $(patsubst %.cpp,%,$(SRC))

$(TARGET) : $(SRC)
	$(CC) -DLINUX $(CXXFLAGS) -I$(INC_DIR) $< $(LINKAGE) -o $@

.PHONY : clean
clean :
	rm -f $(TARGET)
