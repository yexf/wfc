############ makefile ;; do with mgwin
ROOTDIR := .

SRCDIRS := $(ROOTDIR)

SRCEXTS := .cpp .c .cc

-include *.mk

OUTDIR := $(ROOTDIR)/build
DEBUG_DIR := $(OUTDIR)/debug/
RELEASE_DIR := $(OUTDIR)/release/
ifeq ($(DEBUG), 1)
OBJDIR := $(DEBUG_DIR)
DEBUGFLAGS := -g
else
OBJDIR := $(RELEASE_DIR)
DEBUGFLAGS :=
endif
PROGRAMDIR := $(ROOTDIR)
PROGRAM := $(PROGRAMDIR)/$(PROGRAM)

# 预处理阶段的选项
CPPFLAGS := $(CPPFLAGS)
INLIBS := $(INLIBS) 
LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS)
CXXFLAGS := $(CXXFLAGS) $(DEBUGFLAGS)

CC = gcc
CXX = g++
AR = ar
RM = rm -f

SHELL = /bin/sh
ifeq ($(SUBDIRS), )
# SUBDIRS := $(shell ls -l | grep ^d | awk '{if($$9 != "build" && $$9 != "Debug" && $$9 != "Release")}' print $$9}')
SUBDIRS := $(shell find . -type d | grep -v ^./build | grep -v ^./Debug | grep -v ^./Release)
else
SUBDIRS := $(SUBDIRS)
endif
FULLSOURCES =$(foreach s,$(SUBDIRS),$(foreach d,$(s),$(wildcard $(addprefix $(d)/*,$(SRCEXTS)))))
OBJS = $(foreach x,$(SRCEXTS), $(patsubst %$(x), %.o,$(filter %$(x),$(FULLSOURCES))))
FULLOBJS = $(addprefix $(OBJDIR),$(OBJS))
DEPS = $(patsubst %.o,%.d,$(FULLOBJS))
MKFILE = $(wildcard $(addprefix $(ROOTDIR)/*,.mk))

vpath %.c $(SRCDIRS)
vpath %.cpp $(SRCDIRS)
vpath %.o $(OBJDIR)

.PHONY : all objs clean cleanall rebuild link cleanout test ECHO  

all : config $(PROGRAM) 


objs : $(OBJS)

$(OBJDIR)%.o : %.c $(MKFILE)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)%.o : %.cc $(MKFILE)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
    
$(OBJDIR)%.o : %.cpp $(MKFILE)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
    
$(PROGRAM) : $(FULLOBJS)
ifeq ($(findstring .a, $(PROGRAM)), .a)
	$(AR) -rc $(PROGRAM) $(FULLOBJS)
else
ifeq ($(strip $(SRCEXTS)), .c) 
	$(CC) -o $(PROGRAM) $(FULLOBJS) $(INLIBS) $(LDFLAGS)
else
	$(CXX) -o $(PROGRAM) $(FULLOBJS) $(INLIBS) $(LDFLAGS)
endif
endif

-include $(DEPS)

ifeq ($(DEBUG), 1)
config:dir
else
config:dir
endif

dir: 
	@echo "makeing dir..."
	mkdir -p $(OUTDIR)
	mkdir -p $(DEBUG_DIR)
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(addprefix $(OBJDIR), $(wildcard $(SUBDIRS)))
	mkdir -p $(PROGRAMDIR)
	
rebuild: clean all

link: cleanout all

cleanout:
	@$(RM) $(PROGRAM)
	
clean:
	@$(RM) -r $(OUTDIR)	
	@$(RM) $(DEPS)	
	@$(RM) $(PROGRAM)	

cleanall: clean cleanout

test: 
	@echo $(SUBDIRS)
#	@echo $(MKFILE)
#	@echo $(FULLOBJS)
#	@echo $(SUBDIRS)
#	@echo $(OBJS)
	
	