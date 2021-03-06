############ wcc makefile ;; do with mgwin
BUILD_FILE:=$(shell pwd)/build.mak
BUILD_DIR:= cef/libcef_dll/ libwfc/ shared/ cefsimple/ wfcsimple/ cefclient/ wfcui/
TEST_TARGET := cefclient/
BUILD_TARGET:= $(addsuffix .target, $(BUILD_DIR))
BUILD_CLEAN:= $(addsuffix .clean, $(BUILD_DIR))
export BUILD_FILE

.PHONY: lua 

all : lua $(BUILD_TARGET)
	
%.target : %
	make -C $< -f config.mak -j
%.clean : %
	make -C $< -f config.mak clean

lua: 
	make -C ./lua mingw
	cp -f ./lua/src/lua51.dll ./bin/lua51.dll
	
rebuild: clean all
	
link: cleanout all

cleanout:
	
clean: $(BUILD_CLEAN)
	
cleanall: clean cleanout

test: all 
	make -C ./lua clean
	make -C $(TEST_TARGET) -f config.mak $@

	
	