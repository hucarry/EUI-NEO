cmake_minimum_required(VERSION 3.14)
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(ModernOpenGLApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(EUI_MESA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../mesa3d" CACHE PATH "Mesa3D root used by VM package targets")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target" FORCE)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    TIMEOUT 30
)
FetchContent_MakeAvailable(glfw)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
FetchContent_Declare(
    glad
    URL https://github.com/libigl/libigl-glad/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    TIMEOUT 30
)
FetchContent_MakeAvailable(glad)

find_package(OpenGL REQUIRED)
if(NOT WIN32)
    find_package(CURL REQUIRED)
else()
    find_package(CURL QUIET)
endif()

set(CORE_SOURCES
    core/image.cpp
    core/network.cpp
    core/platform.cpp
    core/text.cpp
)

set(APP_ICON_RESOURCE)
if(WIN32)
    set(APP_ICON_ICO "${CMAKE_CURRENT_SOURCE_DIR}/assets/icon.ico")
    set(APP_ICON_RC "${CMAKE_CURRENT_BINARY_DIR}/eui_app_icon.rc")
    file(TO_NATIVE_PATH "${APP_ICON_ICO}" APP_ICON_ICO_NATIVE)
    string(REPLACE "\\" "\\\\" APP_ICON_ICO_ESCAPED "${APP_ICON_ICO_NATIVE}")
    file(WRITE "${APP_ICON_RC}" "IDI_APP_ICON ICON \"${APP_ICON_ICO_ESCAPED}\"\n")
    set(APP_ICON_RESOURCE "${APP_ICON_RC}")
endif()

function(configure_opengl_app target_name)
    target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${target_name} PRIVATE glfw glad OpenGL::GL)

    if(MSVC)
        target_compile_options(${target_name} PRIVATE $<$<NOT:$<CONFIG:Debug>>:/O1> /GS- /sdl-)
        target_link_options(${target_name} PRIVATE /OPT:REF /OPT:ICF /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS)
    else()
        target_compile_options(${target_name} PRIVATE -Os -fno-exceptions -fno-rtti)
        target_link_options(${target_name} PRIVATE -Wl,--gc-sections -s)
    endif()

    if(WIN32)
        target_link_libraries(${target_name} PRIVATE winmm urlmon shell32)

        get_target_property(GLFW_TARGET_TYPE glfw TYPE)
        if(GLFW_TARGET_TYPE STREQUAL "SHARED_LIBRARY")
            add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:glfw>
                $<TARGET_FILE_DIR:${target_name}>
            )
        endif()
    endif()

    if(TARGET CURL::libcurl)
        target_link_libraries(${target_name} PRIVATE CURL::libcurl)
        target_compile_definitions(${target_name} PRIVATE EUI_HAS_CURL=1)
    endif()

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${target_name}>/assets
    )
endfunction()

file(GLOB APP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp")
set(EUI_APP_TARGETS)
foreach(app_source ${APP_SOURCES})
    get_filename_component(app_name "${app_source}" NAME_WE)
    list(APPEND EUI_APP_TARGETS ${app_name})
    add_executable(${app_name} main.cpp "${app_source}" ${CORE_SOURCES} ${APP_ICON_RESOURCE})
    configure_opengl_app(${app_name})

    if(WIN32)
        add_custom_target(package_vm_${app_name}
            COMMAND powershell -ExecutionPolicy Bypass -File
                "${CMAKE_CURRENT_SOURCE_DIR}/tools/package-vm.ps1"
                -App ${app_name}
                -Config $<CONFIG>
                -BuildRoot "${CMAKE_BINARY_DIR}"
                -MesaRoot "${EUI_MESA_ROOT}"
            DEPENDS ${app_name}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Packaging ${app_name} with Mesa software OpenGL for VMs"
            VERBATIM
        )
    endif()
endforeach()

if(WIN32)
    set(EUI_VM_PACKAGE_TARGETS)
    foreach(app_name ${EUI_APP_TARGETS})
        list(APPEND EUI_VM_PACKAGE_TARGETS package_vm_${app_name})
    endforeach()

    add_custom_target(package_vm_all
        DEPENDS ${EUI_VM_PACKAGE_TARGETS}
        COMMENT "Packaging all apps with Mesa software OpenGL for VMs"
    )
endif()
