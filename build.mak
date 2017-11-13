############ makefile ;; do with mgwin
ROOTDIR := .
SRCDIRS := $(ROOTDIR)
SRCEXTS := .cpp .c .cc

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
ifeq ($(PROGRAMDIR),)
PROGRAMDIR := $(ROOTDIR)
endif
PROGRAM := $(PROGRAMDIR)/$(PROGRAM)

# 预处理阶段的选项
CPPFLAGS := $(CPPFLAGS)
INLIBS := $(INLIBS) 
DEFLIBS := $(DEFLIBS)
LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS)
CXXFLAGS := $(CXXFLAGS) $(DEBUGFLAGS)

-include common.mak
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
MKFILE = $(wildcard $(addprefix $(ROOTDIR)/*,.mak))

vpath %.c $(SRCDIRS)
vpath %.cpp $(SRCDIRS)
vpath %.o $(OBJDIR)

.PHONY : all objs clean cleanall rebuild link cleanout test ECHO  

all : config $(PROGRAM) 

%.a:%.def
	dlltool -d $< -l $@
	
objs : $(OBJS)

$(OBJDIR)%.o : %.c $(MKFILE)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o : %.cc $(MKFILE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
    
$(OBJDIR)%.o : %.cpp $(MKFILE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
    
$(PROGRAM) : $(FULLOBJS) $(DEFLIBS)
ifeq ($(findstring .a, $(PROGRAM)), .a)
	$(AR) -rc $(PROGRAM) $(FULLOBJS)
else
ifeq ($(strip $(SRCEXTS)), .c) 
	$(CC) $(LDFLAGS) -o $(PROGRAM) $(FULLOBJS) $(INLIBS) $(DEFLIBS)
else
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(FULLOBJS) $(INLIBS) $(DEFLIBS)
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
	@$(RM) $(DEFLIBS)	
	@$(RM) $(PROGRAM)	

cleanall: clean cleanout

test: 
	$(PROGRAM)
#	@echo $(SUBDIRS)
#	@echo $(MKFILE)
#	@echo $(FULLOBJS)
#	@echo $(SUBDIRS)
#	@echo $(OBJS)
	
	