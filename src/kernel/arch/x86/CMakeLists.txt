SET(ARCH_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${ARCH_DIR}/platform/${TARGET_PLATFORM})

set(ARCH_LINKER_FLAGS ${PLATFORM_LINKER_FLAGS})
set(ARCH_C_FLAGS ${PLATFORM_C_FLAGS})
set(ARCH_CXX_FLAGS ${PLATFORM_CXX_FLAGS})
set(ARCH_ASM_FLAGS ${PLATFORM_ASM_FLAGS})

set(ARCH_SRCS
        boot/stackchk.cpp
        boot/x86init.cpp
        boot/x86Platform.h
        boot/x86Platform.cpp
        descriptorTables/gdt.s
        descriptorTables/gdt.h
        descriptorTables/gdt.cpp
        drivers/PCTerminalDriver.h
        drivers/PCTerminalDriver.cpp
        mm/mm.cpp)

# Add the path onto the front of each source file
PREPEND_PATH(ARCH_SRCS ${CMAKE_CURRENT_LIST_DIR} ${ARCH_SRCS})

set(ARCH_SRCS ${PLATFORM_SRCS}
        ${ARCH_SRCS})

# Export variables to parent scope
set(ARCH_LINKER_FLAGS ${ARCH_LINKER_FLAGS} PARENT_SCOPE)
set(ARCH_C_FLAGS ${ARCH_C_FLAGS} PARENT_SCOPE)
set(ARCH_CXX_FLAGS ${ARCH_CXX_FLAGS} PARENT_SCOPE)
set(ARCH_ASM_FLAGS ${ARCH_ASM_FLAGS} PARENT_SCOPE)
set(ARCH_SRCS ${ARCH_SRCS} PARENT_SCOPE)