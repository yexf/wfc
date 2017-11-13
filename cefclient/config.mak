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
PROGRAM := cefclient.exe

# 预处理阶段的选项
CPPFLAGS := -I../ -I../cef -DUNICODE -D_UNICODE -D_WINDOWS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -DWINVER=0x0601 -D_WIN32_WINNT=0x601 -DWIN32_LEAN_AND_MEAN -D_HAS_EXCEPTIONS -DPSAPI_VERSION=1
DEFLIBS := ../lib/libcef.a 
INLIBS := -lmingw32 -lcef_dll_wrapper ../shared/libshared.a -lkernel32 -luser32 -lgdi32 -lshell32 -limm32 -lwinspool -lole32 -loleaut32 -lcomdlg32 -ladvapi32 -lcomctl32 -lrpcrt4 -lshlwapi -lws2_32 -lglu32 -lopengl32 -luuid
LDFLAGS := -mwindows -municode -static -L../lib -L../cef/libcef_dll -Wl,--gc-sections
CFLAGS := 
CXXFLAGS := -std=c++11 -fpermissive -ffunction-sections -fdata-sections
include $(BUILD_FILE)
