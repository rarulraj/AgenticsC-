cmake_minimum_required(VERSION 3.20)
project(RepoRefactorer VERSION 1.0.0 LANGUAGES CXX)

# Minimal build option for bootstrapping without all deps
option(MINIMAL_BUILD "Build only minimal executable (src/main.cpp)" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find packages only if not minimal
if(NOT MINIMAL_BUILD)
    find_package(PkgConfig REQUIRED)
    find_package(Threads REQUIRED)

    # Clang/LLVM
    find_package(LLVM REQUIRED CONFIG)
    find_package(Clang REQUIRED CONFIG)

    # Third-party libraries
    find_package(spdlog REQUIRED)
    find_package(fmt REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(yaml-cpp REQUIRED)
    find_package(SQLite3 REQUIRED)
    find_package(GTest REQUIRED)
    find_package(benchmark REQUIRED)

    # Git library
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBGIT2 REQUIRED libgit2)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MINIMAL_BUILD)
    set(SOURCES
        src/main.cpp
    )
else()
    # Source files
    set(SOURCES
        src/main.cpp
        src/core/agent.cpp
        src/core/planner.cpp
        src/core/context.cpp
        src/agents/build_agent.cpp
        src/agents/ast_agent.cpp
        src/agents/rewrite_agent.cpp
        src/agents/test_agent.cpp
        src/agents/perf_agent.cpp
        src/agents/vcs_agent.cpp
        src/refactor/refactor_pass.cpp
        src/refactor/modernize_passes.cpp
        src/knowledge/knowledge_base.cpp
        src/config/config_manager.cpp
    )
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

if(NOT MINIMAL_BUILD)
    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        spdlog::spdlog
        fmt::fmt
        nlohmann_json::nlohmann_json
        yaml-cpp
        SQLite::SQLite3
        GTest::gtest
        GTest::gtest_main
        benchmark::benchmark
        ${LIBGIT2_LIBRARIES}
        ${LLVM_LIBRARIES}
        ${CLANG_LIBRARIES}
        Threads::Threads
    )

    # Include directories for third-party
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBGIT2_INCLUDE_DIRS})

    # Compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE ${LIBGIT2_CFLAGS_OTHER})
endif()

# Test configuration (only if tests directory exists and not minimal)
if(NOT MINIMAL_BUILD)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()

