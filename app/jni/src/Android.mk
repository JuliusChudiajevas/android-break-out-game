LOCAL_PATH := $(call my-dir)
LOCAL_SHORT_COMMANDS := true

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMAGE_PATH := ../SDL2_image
SDL_TTF_PATH := ../SDL2_ttf
ENTT_PATH := ../entt
INCLUDES_PATH := ../include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(INCLUDES_PATH) $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_IMAGE_PATH) $(LOCAL_PATH)/$(SDL_TTF_PATH) $(LOCAL_PATH)/$(ENTT_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp Game.cpp Scene.cpp TextureManager.cpp TickTimer.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf SDL2_image
LOCAL_CFLAGS := -std=c++17

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
