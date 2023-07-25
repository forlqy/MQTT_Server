cmake_minimum_required(VERSION 3.5)
project(MQTTServer)

set(CMAKE_CXX_STANDARD 11)

# 添加源文件
set(SOURCES src/main.cpp)

find_library(MOSQUITTO_LIBRARY mosquitto)

# 添加包含目录
include_directories(/usr/local/include /usr/include/librdkafka)
# 添加库目录
link_directories(/usr/local/lib)

# 添加可执行文件
add_executable(MQTTServer ${SOURCES})

# 设置可执行文件输出路径
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 链接所需的库
target_link_libraries(MQTTServer ${MOSQUITTO_LIBRARY} yaml-cpp pthread jsoncpp rdkafka++ cpp_redis tacopie)

