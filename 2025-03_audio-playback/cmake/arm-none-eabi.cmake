if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()


set(TOOLCHAIN_PREFIX arm-none-eabi-)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)


# Note, -Wpedantic makes stm32f4 ll driver compilation fail
set(WARNINGS
    "-Wall -Wextra -Wfatal-errors -Wno-unused-parameter -Werror")
set(C_FLAGS "-fdata-sections -ffunction-sections -MMD -MP")
set(LINK_FLAGS "--specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage")


set(CMAKE_ASM_FLAGS_INIT
    "-x assembler-with-cpp"
    CACHE INTERNAL "asm compiler flags")

set(CMAKE_C_FLAGS_INIT
    "${CPU_FLAGS} ${C_FLAGS} ${WARNINGS}"
    CACHE INTERNAL "c compiler flags")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${CPU_FLAGS} ${LINK_FLAGS}"
    CACHE INTERNAL "executable linker flags")

set(CMAKE_C_FLAGS_DEBUG
    "-Og -g3 -gdwarf-5"
    CACHE INTERNAL "c compiler debug flags")

set(CMAKE_C_FLAGS_RELEASE
    "-Og"
    CACHE INTERNAL "c compiler release flags")

set(CMAKE_C_FLAGS_MINSIZEREL
    "-Os"
    CACHE INTERNAL "c compiler min-size release flags")

set(CMAKE_C_FLAGS_RELWITHDEBINFO
    "-Og -g3 -gdwarf-5"
    CACHE INTERNAL "c compiler release with debug info")

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)

# execute_process(
#   COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
#   OUTPUT_VARIABLE BINUTILS_PATH
#   OUTPUT_STRIP_TRAILING_WHITESPACE
# )

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# get_filename_component(ARM_TOOLCHAIN_DIR ${BINUTILS_PATH} DIRECTORY)
# set(CMAKE_OBJCOPY ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
# set(CMAKE_SIZE_UTIL ${ARM_TOOLCHAIN_DIR}/${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")

# set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
