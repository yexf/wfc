# WIN32
# _WINDOWS
# _DEBUG
# __STDC_CONSTANT_MACROS
# __STDC_FORMAT_MACROS
# _WIN32
# UNICODE
# _UNICODE
# WINVER=0x0601
# _WIN32_WINNT=0x601
# NOMINMAX
# WIN32_LEAN_AND_MEAN
# _HAS_EXCEPTIONS=0
# PSAPI_VERSION=1
# CEF_USE_ATL


PROGRAMDIR := ../bin
PROGRAM := wfcui.exe

# 预处理阶段的选项
CPPFLAGS := -I../cef 
LDFLAGS := -static -L../bin -l:libwfc.dll
CFLAGS := 
CXXFLAGS := -std=c++11 -fpermissive
include $(BUILD_FILE)
