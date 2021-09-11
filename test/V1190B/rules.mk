TOP_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
INC_DIR     := $(TOP_DIR)/../../inc
LINKAGE     := -lVMEplusM -lVMEplus

CC          := g++
CXXFLAGS    := -std=c++11

SRC     := $(wildcard *.cpp)
TARGET  := $(patsubst %.cpp,%,$(SRC))

$(TARGET) : $(SRC)
	@echo "MAKEFILE_LIST = \"$(MAKEFILE_LIST)\""
	$(CC) -DLINUX $(CXXFLAGS) -I$(INC_DIR) $< $(LINKAGE) -o $@

.PHONY : clean
clean :
	rm -f $(TARGET)
