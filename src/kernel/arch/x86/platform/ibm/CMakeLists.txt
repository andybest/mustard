set(PLATFORM_DIR ${CMAKE_CURRENT_LIST_DIR})
set(PLATFORM_LINKER_FLAGS "-T ${CMAKE_CURRENT_LIST_DIR}/linker.ld")
set(PLATFORM_C_FLAGS "-target i386-none-elf -march=i386 -g -fbuiltin -fno-omit-frame-pointer")
set(PLATFORM_CXX_FLAGS "-target i386-none-elf -march=i386 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-threadsafe-statics")
set(PLATFORM_ASM_FLAGS "-target i386-none-elf -march=i386 -g -fbuiltin -fno-omit-frame-pointer")

set(PLATFORM_SRCS boot.S)

# Add the path onto the front of each source file
PREPEND_PATH(PLATFORM_SRCS ${CMAKE_CURRENT_LIST_DIR} ${PLATFORM_SRCS})

set(PLATFORM_LINKER_FLAGS ${PLATFORM_LINKER_FLAGS} PARENT_SCOPE)
set(PLATFORM_C_FLAGS ${PLATFORM_C_FLAGS} PARENT_SCOPE)
set(PLATFORM_CXX_FLAGS ${PLATFORM_CXX_FLAGS} PARENT_SCOPE)
set(PLATFORM_ASM_FLAGS ${PLATFORM_ASM_FLAGS} PARENT_SCOPE)
set(PLATFORM_SRCS ${PLATFORM_SRCS} PARENT_SCOPE)