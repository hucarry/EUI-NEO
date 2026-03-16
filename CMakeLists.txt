cmake_minimum_required(VERSION 3.20)
project(EUI VERSION 0.1.0 LANGUAGES C CXX)

option(EUI_BUILD_EXAMPLES "Build GLFW + OpenGL demo when dependencies exist" ON)
option(EUI_STRICT_WARNINGS "Enable strict warnings on local targets" ON)
option(EUI_FETCH_GLFW_FROM_GIT "Auto-fetch GLFW from Git if it is not available locally" ON)
set(EUI_GLFW_GIT_TAG "3.4" CACHE STRING "GLFW Git tag used by FetchContent")

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(eui INTERFACE)
add_library(EUI::eui ALIAS eui)
target_include_directories(eui INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(eui INTERFACE cxx_std_17)

if(EUI_STRICT_WARNINGS)
    if(MSVC)
        target_compile_options(eui INTERFACE /W4)
    else()
        target_compile_options(eui INTERFACE -Wall -Wextra -Wpedantic)
    endif()
endif()

if(EUI_BUILD_EXAMPLES)
    include(FetchContent)

    set(EUI_OPENGL_TARGET "")
    find_package(OpenGL QUIET)
    if(TARGET OpenGL::GL)
        set(EUI_OPENGL_TARGET OpenGL::GL)
    elseif(WIN32)
        # On Windows OpenGL comes from the system SDK.
        set(EUI_OPENGL_TARGET opengl32)
    elseif(APPLE)
        find_library(EUI_OPENGL_FRAMEWORK OpenGL)
        if(EUI_OPENGL_FRAMEWORK)
            set(EUI_OPENGL_TARGET ${EUI_OPENGL_FRAMEWORK})
        endif()
    endif()

    find_package(glfw3 CONFIG QUIET)
    if(NOT TARGET glfw AND NOT TARGET glfw3::glfw AND NOT TARGET glfw3)
        find_package(glfw3 QUIET)
    endif()

    set(EUI_GLFW_TARGET "")
    if(TARGET glfw)
        set(EUI_GLFW_TARGET glfw)
    elseif(TARGET glfw3::glfw)
        set(EUI_GLFW_TARGET glfw3::glfw)
    elseif(TARGET glfw3)
        set(EUI_GLFW_TARGET glfw3)
    endif()

    if(NOT EUI_GLFW_TARGET AND EUI_FETCH_GLFW_FROM_GIT)
        message(STATUS "GLFW not found locally. Fetching from Git (${EUI_GLFW_GIT_TAG})...")

        set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "Disable GLFW install target" FORCE)

        FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG ${EUI_GLFW_GIT_TAG}
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(glfw)

        if(TARGET glfw)
            set(EUI_GLFW_TARGET glfw)
        elseif(TARGET glfw3::glfw)
            set(EUI_GLFW_TARGET glfw3::glfw)
        elseif(TARGET glfw3)
            set(EUI_GLFW_TARGET glfw3)
        endif()
    endif()

    if(EUI_OPENGL_TARGET AND EUI_GLFW_TARGET)
        add_executable(eui_demo WIN32 examples/basic_demo.cpp)
        target_link_libraries(eui_demo PRIVATE EUI::eui ${EUI_GLFW_TARGET} ${EUI_OPENGL_TARGET})
        target_compile_definitions(eui_demo PRIVATE EUI_ENABLE_GLFW_OPENGL_BACKEND=1)
        if(MSVC)
            target_compile_options(eui_demo PRIVATE /W4)
        else()
            target_compile_options(eui_demo PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        add_executable(eui_calculator_demo WIN32 examples/calculator_demo.cpp)
        target_link_libraries(eui_calculator_demo PRIVATE EUI::eui ${EUI_GLFW_TARGET} ${EUI_OPENGL_TARGET})
        target_compile_definitions(eui_calculator_demo PRIVATE EUI_ENABLE_GLFW_OPENGL_BACKEND=1)
        if(MSVC)
            target_compile_options(eui_calculator_demo PRIVATE /W4)
        else()
            target_compile_options(eui_calculator_demo PRIVATE -Wall -Wextra -Wpedantic)
        endif()
    elseif(NOT EUI_OPENGL_TARGET)
        message(WARNING
            "Skipping eui_demo: OpenGL library not found on this platform."
        )
    elseif(NOT EUI_GLFW_TARGET)
        message(WARNING
            "Skipping eui_demo: GLFW not found and Git fetch is disabled/failed. "
            "Enable EUI_FETCH_GLFW_FROM_GIT or install GLFW locally."
        )
    endif()
endif()
