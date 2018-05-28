# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../

LOCAL_MODULE     :=  CoreGuiToolkit
LOCAL_SRC_FILES  := 	CoreGuiToolkit.cpp \
			BaseGuiElements/CoreGuiBase.cpp \
			BaseGuiElements/CoreGuiButton.cpp \
			BaseGuiElements/CoreGuiFrame.cpp \
      BaseGuiElements/CoreGuiTextfield.cpp \
      BaseGuiElements/CoreGuiInputfield.cpp \
      BaseGuiElements/CoreGuiImageView.cpp \
      GuiElements/CoreGuiDialog.cpp \
      GuiElements/CoreGuiWindow.cpp \
      GuiElements/CoreGuiView.cpp

LOCAL_STATIC_LIBRARIES := CoreRendering

include $(BUILD_STATIC_LIBRARY)
