cmake_minimum_required(VERSION 4.0)
project(RBFA_project VERSION 0.1.0)

include(cTest)
enable_testing()

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})




add_executable(RBFA_project main_source_code.cpp)

target_link_libraries(RBFA_project ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)