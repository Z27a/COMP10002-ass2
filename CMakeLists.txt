cmake_minimum_required(VERSION 3.16)
project(COMP10002_ass2 C)

set(CMAKE_C_STANDARD 99)

add_executable(COMP10002_ass2 main.c ass2.h stage0.c stage1.c)
add_executable(submission submission.c)
