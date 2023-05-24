cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Bezier-x86)
enable_language(ASM_NASM)

add_executable(bezier bezier.asm main.c)