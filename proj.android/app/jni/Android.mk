LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Cocos2d-x import paths
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

# 项目根路径变量，便于维护
GAME_ROOT := $(LOCAL_PATH)/../../..

# 所有源文件
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(GAME_ROOT)/Classes/AppDelegate.cpp \
                   $(GAME_ROOT)/Classes/HelloWorldScene.cpp \
                   $(GAME_ROOT)/Classes/views/PlayCardMainViewScene.cpp \
                   $(GAME_ROOT)/Classes/views/GameViewScene.cpp \
                   $(GAME_ROOT)/Classes/views/CardViewSceneItem.cpp \
                   $(GAME_ROOT)/Classes/views/PlayFieldView.cpp \
                   $(GAME_ROOT)/Classes/controller/GameController.cpp \
                   $(GAME_ROOT)/Classes/controller/PlayFieldController.cpp \
                   $(GAME_ROOT)/Classes/controller/StackController.cpp \
                   $(GAME_ROOT)/Classes/controller/GameModelFromLevelGenerator.cpp \
                   $(GAME_ROOT)/Classes/controller/LevelConfigLoader.cpp \
                   $(GAME_ROOT)/Classes/manager/UndoManager.cpp

# 头文件搜索路径
LOCAL_C_INCLUDES := $(GAME_ROOT)/Classes \
                    $(GAME_ROOT)/Classes/views \
                    $(GAME_ROOT)/Classes/controller \
                    $(GAME_ROOT)/Classes/manager \
                    $(GAME_ROOT)/Classes/configs \
                    $(GAME_ROOT)/Classes/configs/models

# C++11 支持
LOCAL_CPPFLAGS := -std=c++14 -frtti -fexceptions

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../cocos2d)
$(call import-module, cocos)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
