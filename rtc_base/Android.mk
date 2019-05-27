
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rtcbase

LOCAL_CFLAGS = -D__ANDROID__ -D__native__client -DWEBRTC_ANDROID -DWEBRTC_POSIX

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../ThirdPary/RtcBase/src

LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/*.cc)

LOCAL_ARM_MODE := arm
LOCAL_CPP_FEATURES := rtti exceptions

include $(BUILD_STATIC_LIBRARY)
