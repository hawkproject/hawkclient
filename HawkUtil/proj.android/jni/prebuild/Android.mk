LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := hawkutil_static
LOCAL_MODULE_FILENAME := libhawkutil
LOCAL_SRC_FILES := ./libs/$(TARGET_ARCH_ABI)/libhawkutil.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
include $(PREBUILT_STATIC_LIBRARY)
