cmake_minimum_required (VERSION 2.6)

project(jetson_hw_encoder)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

find_package(NVMMAPI)

include_directories(include)

set(SOURCES STATIC ${SOURCES}
    src/NvBuffer.cpp
    src/NvElement.cpp
    src/NvElementProfiler.cpp
    src/NvVideoEncoder.cpp
    src/NvV4l2Element.cpp
    src/NvV4l2ElementPlane.cpp            
    src/NvLogging.cpp
    src/NvUtils.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
         
target_link_libraries(${PROJECT_NAME} 
    ${NVMMAPI_LIBRARIES} pthread v4l2)         