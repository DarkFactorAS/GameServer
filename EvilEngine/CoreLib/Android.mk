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
# Documentaiton for makefiles :
# https://docs.google.com/document/d/1jDmWgVgorTY_njX68juH5vt0KY_FXWgxkxmi2v_W_a4/edit
#
#
LOCAL_PATH   := $(call my-dir)

LOCAL_C_INCLUDES += $(MY_LOCAL_PATH)/..
LOCAL_C_INCLUDES += $(MY_LOCAL_PATH)/../..

include $(CLEAR_VARS)

LOCAL_MODULE     :=  CoreLib
LOCAL_SRC_FILES  :=     BasicTypes/BinaryStream/BinaryStream.cpp \
                        BasicTypes/String/CoreString.cpp \
                        BasicTypes/String/StringList.cpp \
                        CRC32/crc32.cpp \
                        CoreMath/Matrix.cpp \
                        CoreMath/Vector3.cpp \
                        MD5/md5.cpp 

include $(BUILD_STATIC_LIBRARY)
