cmake_minimum_required(VERSION 3.15)
project(NWSForecast)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

set(${CMAKE_PREFIX_PATH} ~/.conan2/p)
include_directories(${CMAKE_PREFIX_PATH}/include)

add_executable(nws nws.cpp include/Functions.h include/Classes.h)
target_link_libraries(nws ${CONAN_PKG_CONFIG_LIBRARIES} CURL::libcurl nlohmann_json::nlohmann_json)

# Copy zips.csv to the build directory
add_custom_command(
    TARGET nws POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/zips.csv
            $<TARGET_FILE_DIR:nws>/zips.csv
)
