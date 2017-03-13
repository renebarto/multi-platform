project(osal CXX)

include(${CMAKE_SOURCE_DIR}/cmake/setup_target_properties_library.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/show_target_properties.cmake)

message("\nosal\n")

find_package(Threads REQUIRED)

set(COMPONENTS_OS_DEFINITIONS
        OSAL_DYNAMIC)

set(COMPONENTS_OSAL_INCLUDE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/export)

if (WIN_MSVC OR MINGW)
    set(OSAL_DEPENDENCIES
            Dbghelp.lib)
elseif (LINUX OR APPLE)
    set(OSAL_DEPENDENCIES
            )
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

set(COMPONENTS_OSAL_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${OSAL_DEPENDENCIES})

file(GLOB COMPONENTS_OSAL_SOURCES_BASE src/*.cpp)
file(GLOB COMPONENTS_OSAL_INCLUDES_EXPORT_BASE export/osal/*.h)
file(GLOB COMPONENTS_OSAL_INCLUDES_BASE include/osal/*.h)

if (WIN_MSVC)
    message(STATUS "Building on Windows with MSVC")
    file(GLOB COMPONENTS_OSAL_SOURCES src/windows/*.cpp)
    file(GLOB COMPONENTS_OSAL_INCLUDES_EXPORT export/osal/windows/*.h)
    file(GLOB COMPONENTS_OSAL_INCLUDES include/osal/windows/*.h)
elseif(MINGW)
    message(STATUS "Building on Windows with MinGW")
    file(GLOB COMPONENTS_OSAL_SOURCES src/mingw/*.cpp)
    file(GLOB COMPONENTS_OSAL_INCLUDES_EXPORT export/osal/mingw/*.h)
    file(GLOB COMPONENTS_OSAL_INCLUDES include/osal/mingw/*.h)
elseif(LINUX)
    message(STATUS "Building on Linux")
    file(GLOB COMPONENTS_OSAL_SOURCES src/linux/*.cpp)
    file(GLOB COMPONENTS_OSAL_INCLUDES_EXPORT export/osal/linux/*.h)
    file(GLOB COMPONENTS_OSAL_INCLUDES include/osal/linux/*.h)
elseif(APPLE)
    message(STATUS "Building on OS X")
    file(GLOB COMPONENTS_OSAL_SOURCES src/darwin/*.cpp)
    file(GLOB COMPONENTS_OSAL_INCLUDES_EXPORT export/osal/darwin/*.h)
    file(GLOB COMPONENTS_OSAL_INCLUDES include/osal/darwin/*.h)
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    _return()
endif()

set(COMPONENTS_OSAL_SOURCES
        ${COMPONENTS_OSAL_SOURCES_BASE}
        ${COMPONENTS_OSAL_SOURCES}
        ${COMPONENTS_OSAL_INCLUDES_EXPORT_BASE}
        ${COMPONENTS_OSAL_INCLUDES_EXPORT}
        ${COMPONENTS_OSAL_INCLUDES_BASE}
        ${COMPONENTS_OSAL_INCLUDES})

MESSAGE(STATUS "CMAKE_CXX_FLAGS:                    " ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG:              " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE:            " ${CMAKE_CXX_FLAGS_RELEASE} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:         " ${CMAKE_CXX_FLAGS_MINSIZEREL} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:     " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} )
list_to_string(COMPONENTS_OSAL_INCLUDE STR)
MESSAGE(STATUS "Include files:                      " ${STR} )
list_to_string(COMPONENTS_OSAL_SOURCES STR)
MESSAGE(STATUS "Source files:                       " ${STR} )
list_to_string(COMPONENTS_OSAL_LIBS STR)
MESSAGE(STATUS "Link libraries:                     " ${STR} )

# add the library
add_library(${PROJECT_NAME} SHARED ${COMPONENTS_OSAL_SOURCES})
target_link_libraries(${PROJECT_NAME} ${COMPONENTS_OSAL_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPONENTS_OSAL_INCLUDE})
target_compile_definitions(${PROJECT_NAME} PRIVATE OSAL_BUILD)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPONENTS_OSAL_DEFINITIONS})
setup_target_properties_library(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

