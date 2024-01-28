cmake_minimum_required(VERSION 3.26)
project(lzt_hw02_HistogramEqualization)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory("./" PROJECT_SOURCES)
add_executable(lzt_hw02_HistogramEqualization ${PROJECT_SOURCES}
        Column.h)


find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries(lzt_hw02_HistogramEqualization ${OpenCV_LIBS})
