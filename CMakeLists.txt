cmake_minimum_required(VERSION 3.16)

project(BinarySerializator)

if(UNIX)
	message("UNIX")
	set(CMAKE_CXX_FLAGS "-std=c++20")
elseif(WIN32)
	message("WIN32")
	set(CMAKE_CXX_FLAGS "/std:c++20 /EHa")
endif()

include_directories(
	include/*.h
	include/*.hpp
)

set(
	SOURCES 
	src/main.cpp
	src/serializator.cpp)

add_executable(binary_serializator ${SOURCES})
