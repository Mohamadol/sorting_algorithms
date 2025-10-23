cmake_minimum_required(VERSION 3.20)

# ---- Project ----
project(SortingHarness
  VERSION 0.1.0
  LANGUAGES CXX
)

# ---- Options ----
option(NATIVE_ARCH         "Enable -march=native (or /arch:AVX2 on MSVC if available)" ON)
option(ENABLE_ASAN         "Enable AddressSanitizer (Debug builds)" OFF)
option(ENABLE_UBSAN        "Enable UndefinedBehaviorSanitizer (Debug builds)" OFF)
option(WARNINGS_AS_ERRORS  "Treat warnings as errors" OFF)
option(ENABLE_IPO          "Try to enable interprocedural optimization (LTO)" ON)

# Enable compile_commands.json (useful for tooling)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- Target: sorter executable ----
add_executable(sorter
  main.cpp
  sorting.cpp
)
target_include_directories(sorter PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---- C++ standard ----
target_compile_features(sorter PRIVATE cxx_std_20)

# ---- Warnings (portable) ----
if(MSVC)
  target_compile_options(sorter PRIVATE /W4)
  if(WARNINGS_AS_ERRORS)
    target_compile_options(sorter PRIVATE /WX)
  endif()
else()
  target_compile_options(sorter PRIVATE
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion
    -Wshadow -Wfloat-conversion
  )
  if(WARNINGS_AS_ERRORS)
    target_compile_options(sorter PRIVATE -Werror)
  endif()
endif()

# ---- Native arch (best effort) ----
if(NATIVE_ARCH)
  if(MSVC)
    # Best-effort: try AVX2 if available
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("/arch:AVX2" HAS_AVX2)
    if(HAS_AVX2)
      target_compile_options(sorter PRIVATE /arch:AVX2)
    endif()
  else()
    target_compile_options(sorter PRIVATE -march=native)
  endif()
endif()

# ---- Sanitizers (Debug-only recommended) ----
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  if(ENABLE_ASAN AND NOT MSVC)
    target_compile_options(sorter PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(sorter    PRIVATE -fsanitize=address)
  endif()
  if(ENABLE_UBSAN AND NOT MSVC)
    target_compile_options(sorter PRIVATE -fsanitize=undefined -fno-omit-frame-pointer)
    target_link_options(sorter    PRIVATE -fsanitize=undefined)
  endif()
endif()

# ---- IPO/LTO (best effort) ----
if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT ipo_supported OUTPUT ipo_msg)
  if(ipo_supported)
    set_property(TARGET sorter PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(STATUS "IPO/LTO not supported: ${ipo_msg}")
  endif()
endif()

# ---- Install (optional) ----
install(TARGETS sorter RUNTIME DESTINATION bin)

# ---- Testing (simple smoke tests) ----
include(CTest) # enables BUILD_TESTING option
if(BUILD_TESTING)
  # Tiny runs just to ensure it executes and returns 0
  add_test(NAME run_std_sort_random
           COMMAND sorter --algo std --pattern random --n 2048 --trials 1)
  add_test(NAME run_insertion_nearly
           COMMAND sorter --algo insertion --pattern nearly --n 2048 --trials 1)
endif()
