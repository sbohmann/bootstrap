cmake_minimum_required(VERSION 3.25)
project(bootstrap)

set(CMAKE_CXX_STANDARD 17)

add_executable(bootstrap main.c template_engine/template_engine.c template_engine/template_engine.h template_engine/replacements.c template_engine/replacements.h)
