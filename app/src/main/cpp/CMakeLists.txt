#
# Copyright (C) 2019 The Android Open Source Project
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

cmake_minimum_required(VERSION 3.6)
project(app VERSION 1.0.0 LANGUAGES CXX)

find_package(curl REQUIRED CONFIG)
find_package(jsoncpp REQUIRED CONFIG)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

add_library(app SHARED app.cpp https.cpp logging.cpp Book.cpp BookManager.cpp)
set_target_properties(app PROPERTIES CXX_STANDARD 17)
target_compile_options(app PRIVATE -Wall -Werror)
target_link_libraries(app ${log-lib} curl::curl jsoncpp::jsoncpp -Wl,--no-undefined)
