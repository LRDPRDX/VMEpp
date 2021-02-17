#export TOP_DIR 	:= $(dir $(firstword $(MAKEFILE_LIST)))
export TOP_DIR 	:= $(shell pwd)
export OBJ_DIR 	:= $(TOP_DIR)/obj
export INC_DIR 	:= $(TOP_DIR)/inc
export SRC_DIR 	:= $(TOP_DIR)/src
export BIN_DIR 	:= $(TOP_DIR)/bin
export LIB_DIR 	:= $(TOP_DIR)/lib
TMPL_DIR 		:= $(TOP_DIR)/template

export HDRS := $(wildcard $(INC_DIR)/*.h)

export CC 	   	:= g++
export CXXFLAGS	:= -DLINUX -std=c++1y -Wall -Wextra -fPIC 
export LINKAGE  := -lCAENVME
export LDFLAGS  := -shared

export MKDIR_P  := mkdir -p

MAKE_TMPL 		:= $(TMPL_DIR)/Makefile
MAIN_TMPL 		:= $(TMPL_DIR)/Main.cpp

all : directories base modules template

.PHONY : directories
directories : $(BIN_DIR) $(LIB_DIR)

$(BIN_DIR) :
	$(MKDIR_P) $@

$(LIB_DIR) :
	$(MKDIR_P) $@

#********BASE************
.PHONY : base
base : 
	$(MAKE) -C $(SRC_DIR)

#********MODULES*********
.PHONY : modules
modules :
	$(MAKE) -C $(SRC_DIR)/modules

#********TEMPLATE********
.PHONY : template
template : $(TMPL_DIR) $(MAKE_TMPL) $(MAIN_TMPL)

$(TMPL_DIR) :
	$(MKDIR_P) $@

$(MAKE_TMPL) : $(SRC_DIR)/.Makefile_tmpl
	printf "TOP_DIR := %s\n" $(TOP_DIR) > $@
	cat $< >> $@

$(MAIN_TMPL) : $(SRC_DIR)/.Main_tmpl
	cat $< > $@

#********INSTALL********
.PHONY : install
install :
	$(MAKE) -C $(SRC_DIR) install
	$(MAKE) -C $(SRC_DIR)/modules install


#********CLEANING*********
.PHONY : clean_base
clean_base :
	$(MAKE) -C $(SRC_DIR) clean	
.PHONY : clean_modules
clean_modules :
	$(MAKE) -C $(SRC_DIR)/modules clean

.PHONY : clean_template
clean_template :
	rm -rf $(TMPL_DIR)

.PHONY : clean
clean : clean_modules clean_base clean_template
