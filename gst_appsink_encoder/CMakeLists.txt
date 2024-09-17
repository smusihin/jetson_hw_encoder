cmake_minimum_required(VERSION 3.5)

project(gst-appsink-encoder)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_PLUGINS REQUIRED gstreamer-plugins-base-1.0)
pkg_check_modules(GSTREAMER_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GSTREAMER_PB_UTILS REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GSTREAMER_RTP REQUIRED gstreamer-rtp-1.0)

include_directories(
    ${GSTREAMER_INCLUDE_DIRS}
    ../jetson_hw_encoder/include
    include
)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    ${GSTREAMER_VIDEO_LIBRARIES}
    ${GSTREAMER_PLUGINS_LIBRARIES}
    ${GSTREAMER_APP_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    jetson_hw_encoder
    )
