############ wcc makefile ;; do with mgwin
ROOTDIR := .

SRCDIRS := $(ROOTDIR)

SRCEXTS := .cpp .c

OUTDIR := $(ROOTDIR)/build
OBJDIR := $(OUTDIR)/
DEBUG_DIR := $(OUTDIR)/debug/
RELEASE_DIR := $(OUTDIR)/release/
PROGRAMDIR := $(ROOTDIR)
PROGRAM := $(PROGRAMDIR)/wcc.exe

CPPFLAGS := $(CPPFLAGS)
INLIBS := $(INLIBS)
LDFLAGS := $(LDFLAGS)
CFLAGS := $(CFLAGS)
CXXFLAGS := $(CFLAGS)

CC = gcc
CXX = g++
AR = ar
RM = rm -f

SHELL = /bin/sh
FULLSOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
SOURCES = $(notdir $(FULLSOURCES))
OBJS = $(foreach x,$(SRCEXTS), $(patsubst %$(x), %.o,$(filter %$(x),$(SOURCES))))
FULLOBJS = $(addprefix $(OBJDIR),$(OBJS))
DEPS = $(patsubst %.o,%.d,$(OBJS))

vpath %.c $(SRCDIRS)
vpath %.cpp $(SRCDIRS)
vpath %.o $(OBJDIR)

.PHONY : all objs clean cleanall rebuild link cleanout test

all : config $(PROGRAM)


objs : $(OBJS)

%.o : %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $(OBJDIR)$(notdir $@)
    
%.o : %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $(OBJDIR)$(notdir $@)
    
$(PROGRAM) : $(OBJS)
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

dir:
	@echo "makeing dir..."
	mkdir -p $(OUTDIR)
# mkdir -p $(DEBUG_DIR)
# mkdir -p $(RELEASE_DIR)
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

test: all
	$(PROGRAM) $@
	
	