
cmake_minimum_required(VERSION 3.4.1)
#[[
#引入ffmpeg头文件
#include_directories(${CMAKE_SOURCE_DIR}/include)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -LF:/AndroidProject/NDKproject/app/libs/${CMAKE_ANDROID_ARCH_ABI}")
#message(${CMAKE_SOURCE_DIR}/libs/${CMAKE_ANDROID_ARCH_ABI}---------------------)
add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_link_libraries( # Specifies the target library.
                        native-lib
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )]]