project(encoder)

include_directories(../jetson_hw_encoder/include)

set(SOURCES video_encode_csvparser.cpp
            video_encode_main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES}) 

target_link_libraries(${PROJECT_NAME} jetson_hw_encoder)