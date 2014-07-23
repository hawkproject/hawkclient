LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#本地模块名
LOCAL_MODULE := hawkutil_static

#生成模块文件名
LOCAL_MODULE_FILENAME := libhawkutil

#定义某个目录下所有cpp文件搜索
define all-cpp-files-under
$(patsubst $(LOCAL_PATH)/%, %, $(shell find $(1) -maxdepth 1 -name "*.cpp" -and -not -name ".*") )
endef

#ICONV参与编译代码文件列表
LOCAL_ICONV_SRC_FILES := \
../../source/iconv/libcharset/lib/localcharset.c \
../../source/iconv/lib/iconv.c \
../../source/iconv/lib/relocatable.c

#SQLITE参与编译源文件列表
LOCAL_SQLITE_SRC_FILES := \
../../source/sqlite/sqlite3.c

#JSON参与编译源文件列表
LOCAL_JSON_SRC_FILES := \
../../source/json/json_reader.cpp \
../../source/json/json_writer.cpp \
../../source/json/json_value.cpp

#ZIP参与编译源文件列表
LOCAL_ZIP_SRC_FILES := \
../../source/zlib/adler32.c \
../../source/zlib/compress.c \
../../source/zlib/crc32.c \
../../source/zlib/deflate.c \
../../source/zlib/infback.c \
../../source/zlib/inffast.c \
../../source/zlib/inflate.c \
../../source/zlib/inftrees.c \
../../source/zlib/ioapi.c \
../../source/zlib/trees.c \
../../source/zlib/uncompr.c \
../../source/zlib/unzip.c \
../../source/zlib/zutil.c

#Hawk参与编译源文件列表
LOCAL_HAWK_SRC_FILES := $(call all-cpp-files-under, $(LOCAL_PATH)/../../)

$(warning $(LOCAL_ICONV_SRC_FILES))
$(warning $(LOCAL_SQLITE_SRC_FILES))
$(warning $(LOCAL_JSON_SRC_FILES))
$(warning $(LOCAL_ZIP_SRC_FILES))
$(warning $(LOCAL_HAWK_SRC_FILES))

#编译使用的源文件列表
LOCAL_SRC_FILES := $(LOCAL_ICONV_SRC_FILES) + \
				   $(LOCAL_SQLITE_SRC_FILES) + \
				   $(LOCAL_JSON_SRC_FILES) + \
				   $(LOCAL_ZIP_SRC_FILES) + \
				   $(LOCAL_HAWK_SRC_FILES)
				   
#包含引用头文件定义
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/../../source/sqlite \
                    $(LOCAL_PATH)/../../source/json \
					$(LOCAL_PATH)/../../source/rapidxml \
                    $(LOCAL_PATH)/../../source/zlib \
					$(LOCAL_PATH)/../../source/iconv/include \
					$(LOCAL_PATH)/../../source/iconv/lib \
					$(LOCAL_PATH)/../../source/iconv/libcharset \
					$(LOCAL_PATH)/../../source/iconv/libcharset/include
					
#导出的头文件路径
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
				
#依赖库文件定义				
LOCAL_LDLIBS := 
#导出依赖库文件定义
LOCAL_EXPORT_LDLIBS :=

#预编译标记定义
LOCAL_CFLAGS := -DUTIL_EXPORT -DANDROID -D_ANDROID -DUSE_FILE32API -D_FILE_OFFSET_BITS=64 -Wno-multichar -DLIBDIR=\"\" -DBUILDING_LIBICONV -DIN_LIBRARY
#导出预编译标记定义
LOCAL_EXPORT_CFLAGS := -DANDROID -D_ANDROID

#添加抛出异常的支持
LOCAL_CPPFLAGS := -fexceptions
#导出添加抛出异常的支持
LOCAL_EXPORT_CPPFLAGS := -fexceptions

#生成静态库
include $(BUILD_STATIC_LIBRARY)
