cmake_minimum_required(VERSION 2.8)

project(e22.notify)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=GBK -finput-charset=UTF-8") # chinese

aux_source_directory(. SRC)

add_executable(${PROJECT_NAME} ${SRC}) 
