

SET(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang CACHE STRING "C Compiler" FORCE)
SET(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang CACHE STRING "C++ Compiler" FORCE)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0.2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_language(ASM)
# enable_language(ASM_NASM)
# if(NOT CMAKE_ASM_NASM_COMPILER_LOADED)
#   message(FATAL_ERROR "NASM not found")
# endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

ADD_SUBDIRECTORY(src/kernel)