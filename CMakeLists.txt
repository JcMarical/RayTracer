cmake_minimum_required(VERSION 3.27)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)



# 设置链接标志为静态链接
set(CMAKE_EXE_LINKER_FLAGS "-static")



add_executable(ClionTest main.cpp
        vec3.h
        color.h
        ray.h)
