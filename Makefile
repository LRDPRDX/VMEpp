export TOP_DIR  := $(shell pwd)
export DEP_DIR  := $(TOP_DIR)/.deps
export OBJ_DIR  := $(TOP_DIR)/obj
export INC_DIR  := $(TOP_DIR)/inc
export SRC_DIR  := $(TOP_DIR)/src
export BIN_DIR  := $(TOP_DIR)/bin
export LIB_DIR  := $(TOP_DIR)/lib
TMPL_DIR        := $(TOP_DIR)/template

export CXX      := g++
export CXXFLAGS := -DLINUX -std=c++1y -Wall -Wextra -fPIC
export DEPFLAGS  = -MMD -MF $(DEP_DIR)/$$(*F).d # use only in recipes !
export LINKAGE  := -lCAENVME
export LDFLAGS  := -shared

MAKE_TMPL       := $(TMPL_DIR)/Makefile
MAIN_TMPL       := $(TMPL_DIR)/Main.cpp

all : base modules template

#******** BASE ************
.PHONY : base
base : 
	$(MAKE) -C $(SRC_DIR)

#******** MODULES *********
.PHONY : modules
modules :
	$(MAKE) -C $(SRC_DIR)/modules

#******** TEMPLATE ********
.PHONY : template
template : $(MAKE_TMPL) $(MAIN_TMPL)
	$(info $n Creating template...)

$(MAKE_TMPL) : $(SRC_DIR)/.Makefile_tmpl | $(TMPL_DIR)
	printf "TOP_DIR := %s\n" $(TOP_DIR) > $@
	cat $< >> $@

$(MAIN_TMPL) : $(SRC_DIR)/.Main_tmpl | $(TMPL_DIR)
	cat $< > $@

$(TMPL_DIR) : ; mkdir $@


#******** INSTALL ********
.PHONY : install
install :
	$(MAKE) -C $(SRC_DIR) install
	$(MAKE) -C $(SRC_DIR)/modules install


#******** CLEANING *********
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
