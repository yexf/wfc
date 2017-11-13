############ wcc makefile ;; do with mgwin

BUILD_LIB:=make -C cef/libcef_dll/ -f config.mak BUILD=$(shell pwd)/build.mak
BUILD_SIMPLE:=make -C wfc_simple/ -f config.mak BUILD=$(shell pwd)/build.mak

all :
	$(BUILD_LIB) -j 12
	$(BUILD_SIMPLE) -j 12
	
rebuild: clean all
	
link: cleanout all

cleanout:
	
clean:
	$(BUILD_LIB) clean
	$(BUILD_SIMPLE) clean
	
cleanall: clean cleanout

test: all

	
	