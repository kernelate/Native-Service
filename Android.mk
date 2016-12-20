LOCAL_PATH:= $(call my-dir)

common_src_files := \
	socket_server_api.c	\
	connectivity.c \
	wpa.c	\
	wifi_ap.c	\
	ldr_ir.c \
	watchdog_Api.c\
	android_ui_Api.c\
	file_operation.c\
	custom_app_api.c\

common_c_includes := \
	$(KERNEL_HEADERS) \
	external/openssl/include

common_shared_libraries := \
	libcutils 	\
	libsysutils \
	libcrypto

include $(CLEAR_VARS)

LOCAL_MODULE:= platformservice

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
	platformservice.c \
	$(common_src_files)

LOCAL_C_INCLUDES := $(common_c_includes)

LOCAL_LDLIBS := -llog

LOCAL_CFLAGS := 

LOCAL_SHARED_LIBRARIES := $(common_shared_libraries)

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
