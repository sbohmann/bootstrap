cmake_minimum_required(VERSION 3.18)
project(bootstrap)

set(CMAKE_CXX_STANDARD 17)

include_directories(".")

add_executable(bootstrap main.c template_engine/template_engine.c template_engine/template_engine.h template_engine/replacements.c template_engine/replacements.h collections/arraylist.c collections/arraylist.h text_buffer/text_buffer.c text_buffer/text_buffer.h text/text_result.h text/read_file.c text/read_file.h)
