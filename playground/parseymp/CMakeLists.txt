project(readymp)
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)
find_package(Zypp REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(readymp_SOURCES main.cpp package.cpp repository.cpp)
add_executable(readymp ${QT_LIBRARIES} ${readymp_SOURCES})

target_link_libraries(readymp ${QT_LIBRARIES} zypp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${qt4_include_dir})
