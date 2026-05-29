# CMake toolchain file for cross-compiling with the bare-metal arm-none-eabi GCC toolchain.
# Used by CI (and usable locally) to prove the headers compile for a real microcontroller target.
#
#   cmake -B build-arm -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake -DNINJAHSM_BUILD_COMPILE_CHECK=ON
#   cmake --build build-arm

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# There is no runtime/syscalls available here, so have CMake's compiler sanity check build a static
# library rather than try (and fail) to link a full executable.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Target a representative Cortex-M core. Section GC flags keep the demo build small.
set(_ninjahsm_arm_flags "-mcpu=cortex-m4 -mthumb -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS_INIT "${_ninjahsm_arm_flags}")
set(CMAKE_CXX_FLAGS_INIT "${_ninjahsm_arm_flags}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
