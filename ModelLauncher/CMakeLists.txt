cmake_minimum_required(VERSION 3.5)
project(ModelLauncher)

set(OpenCV_DIR C:/opencv/opencv/build/x64/vc16/lib)
set(OpenCV_BIN_DIR C:/opencv/opencv/build/x64/vc16/bin)

set(OpenCV_DEBUG_DLL_FILENAME opencv_world490d.dll)
set(OpenCV_RELEASE_DLL_FILENAME opencv_world490.dll)

set(ONNXRUNTIME_DIR C:/onnx/onnxruntime-win-x64-1.17.3)

find_package(OpenCV REQUIRED)

include_directories(include)

file(GLOB_RECURSE CURR_SOURCES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${CURR_SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-DDLL_EXPORTS")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE "${ONNXRUNTIME_DIR}/include")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})

if (WIN32)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} "${ONNXRUNTIME_DIR}/lib/onnxruntime.lib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ONNXRUNTIME_DIR}/lib/onnxruntime.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OpenCV_DIR}/${OpenCV_DLL_FILENAME}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OpenCV_BIN_DIR}/${OpenCV_DEBUG_DLL_FILENAME}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OpenCV_BIN_DIR}/${OpenCV_RELEASE_DLL_FILENAME}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
endif(WIN32)

if (UNIX)
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} "${ONNXRUNTIME_DIR}/lib/libonnxruntime.so")
endif(UNIX)