<<<<<<< HEAD:test/acq_mode/AfterReset/Makefile
TOP_DIR :=  $(shell pwd)/../../..
=======
TOP_DIR :=  $(shell pwd)/../..
>>>>>>> upstream/misc:test/misc/Makefile
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
