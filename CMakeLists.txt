cmake_minimum_required(VERSION 3.17.0)
project(BasicVectorMatrixLib 
    VERSION 0.1.0 
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
