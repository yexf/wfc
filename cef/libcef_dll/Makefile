############ makefile ;; do with mgwin
ROOTDIR := .

SRCDIRS := $(ROOTDIR)

SRCEXTS := .cpp .c .cc

OUTDIR := $(ROOTDIR)/build
OBJDIR := $(OUTDIR)/
DEBUG_DIR := $(OUTDIR)/debug/
RELEASE_DIR := $(OUTDIR)/release/
PROGRAMDIR := $(ROOTDIR)
PROGRAM := $(PROGRAMDIR)/libcef_dll_wrapper.a

# 预处理阶段的选项
CPPFLAGS := $(CPPFLAGS) -I../ -DWRAPPING_CEF_SHARED
INLIBS := $(INLIBS) 
LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS)
CXXFLAGS := $(CXXFLAGS) -std=c++11 -fpermissive

CC = gcc
CXX = g++
AR = ar
RM = rm -f

SHELL = /bin/sh
ifdef (ONLY_CHILD)
SUBDIRS=$(shell ls -l | grep ^d | awk '{if($$9 != "build" && $$9 != "Debug" && $$9 != "Release") print $$9}')
else
SUBDIRS:= . ./base ./cpptoc ./cpptoc/test ./cpptoc/views ./ctocpp/test ./ctocpp/views ./wrapper
endif
FULLSOURCES =$(foreach s,$(SUBDIRS),$(foreach d,$(s),$(wildcard $(addprefix $(d)/*,$(SRCEXTS)))))
OBJS = $(foreach x,$(SRCEXTS), $(patsubst %$(x), %.o,$(filter %$(x),$(FULLSOURCES))))
FULLOBJS = $(addprefix $(OBJDIR),$(OBJS))
DEPS = $(patsubst %.o,%.d,$(FULLOBJS))

vpath %.c $(SRCDIRS)
vpath %.cpp $(SRCDIRS)
vpath %.o $(OBJDIR)

.PHONY : all objs clean cleanall rebuild link cleanout test ECHO $(SUBDIRS) 

all : config $(PROGRAM)


objs : $(OBJS)

$(OBJDIR)%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)%.o : %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@
    
$(OBJDIR)%.o : %.cpp
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

#-include $(DEPS)

ifeq ($(DEBUG), 1)
config:dir
else
config:dir
endif

$(SUBDIRS):
	mkdir -p $(OBJDIR)$@
	
dir:$(SUBDIRS) 
	@echo "makeing dir..."
	mkdir -p $(OUTDIR)
	mkdir -p $(DEBUG_DIR)
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(PROGRAMDIR)
	
rebuild: clean all

link: cleanout all

cleanout:
	@$(RM) $(PROGRAM)
	
clean:
	@$(RM) $(FULLOBJS)	
	@$(RM) -r $(OBJDIR)	
	@$(RM) $(DEPS)	
	@$(RM) $(PROGRAM)	

cleanall: clean cleanout

test: 
	@echo $(FULLOBJS)
	@echo $(SUBDIRS)
	@echo $(OBJS)
	
	