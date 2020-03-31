cmake_minimum_required(VERSION 2.8)

project(e5.parse_table)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=GBK -finput-charset=UTF-8") # chinese

aux_source_directory(. SRC)

add_executable(${PROJECT_NAME} ${SRC}) 

file(COPY ${PROJECT_SOURCE_DIR}/sample.txt DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)