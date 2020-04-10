cmake_minimum_required(VERSION "3.15.0")

project("cppfcgi"
  VERSION "0.1.0"
  DESCRIPTION "FastCGI for modern C++"
  LANGUAGES CXX
  HOMEPAGE_URL "https://github.com/fmorgner/cppfcgi"
)

cmake_policy(SET CMP0069 NEW)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

include("CheckIPOSupported")
include("CMakePackageConfigHelpers")
include("CPack")
include("CTest")
include("DiscoverTests")

check_ipo_supported(RESULT ENABLE_IPO)

### Tools

if(CMAKE_VERSION VERSION_GREATER "3.6")
    find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable"
    )
endif()

### Library

add_library("${PROJECT_NAME}"
  ".clang-format"
  ".clang-tidy"

  "src/error.cpp"
)

set_target_properties("${PROJECT_NAME}" PROPERTIES
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS OFF
  CXX_CLANG_TIDY "${CLANG_TIDY_EXE}"
  INTERPROCEDURAL_OPTIMIZATION ${ENABLE_IPO}
  PUBLIC_HEADER "include/cppfcgi.hpp"
)

target_include_directories("${PROJECT_NAME}" SYSTEM PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/asio/include>
)

target_include_directories("${PROJECT_NAME}" PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_compile_options("${PROJECT_NAME}" PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
  $<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>
)

target_compile_features("${PROJECT_NAME}" PUBLIC
  "cxx_std_17"
)

### Tests

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  option(${PROJECT_NAME}_RUN_TESTS_AFTER_BUILD "Run ${PROJECT_NAME} unit tests after the build finished" OFF)

  add_executable("${PROJECT_NAME}_tests"
    "${PROJECT_SOURCE_DIR}/test/src/driver.cpp"

    "${PROJECT_SOURCE_DIR}/test/src/error_suite.cpp"
  )

  target_include_directories("${PROJECT_NAME}_tests" SYSTEM PRIVATE
    "${PROJECT_SOURCE_DIR}/test/lib/cute"
    "${PROJECT_SOURCE_DIR}/test/lib/kawaii/include"
  )

  target_include_directories("${PROJECT_NAME}_tests" PRIVATE
    "${PROJECT_SOURCE_DIR}/test/include"
  )

  target_link_libraries("${PROJECT_NAME}_tests"
    "${PROJECT_NAME}"
  )

  target_compile_options("${PROJECT_NAME}_tests" PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-pedantic-errors>
  )

  set_target_properties("${PROJECT_NAME}_tests" PROPERTIES
    CXX_EXTENSIONS OFF
  )

  discover_tests(TARGET "${PROJECT_NAME}_tests")

  if(${PROJECT_NAME}_RUN_TESTS_AFTER_BUILD)
    add_custom_command(TARGET "${PROJECT_NAME}_tests"
      POST_BUILD
      DEPENDS "${PROJECT_NAME}_tests"
      COMMAND "${CMAKE_CTEST_COMMAND}"
      ARGS
      "--output-on-failure"
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
      COMMENT "Running unit tests"
    )
  endif()
endif()

### Installation

install(TARGETS "${PROJECT_NAME}"
  EXPORT "${PROJECT_NAME}Targets"
)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/cppfcgi"
  DESTINATION "include"
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY "AnyNewerVersion"
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(EXPORT "${PROJECT_NAME}Targets"
  DESTINATION "lib/cmake/${PROJECT_NAME}"
  NAMESPACE "${PROJECT_NAME}::"
)

### Packaging

set(CPACK_PACKAGE_VENDOR "Felix Morgner")