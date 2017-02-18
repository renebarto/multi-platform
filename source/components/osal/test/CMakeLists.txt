project(osal.test CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\nosal.test\n")

set(COMPONENTS_OSAL_TEST_DEFINITIONS
        TEST_DATA_ROOT="${TEST_DATA_ROOT}"
        CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

set(COMPONENTS_OSAL_TEST_INCLUDE
        ${PROJECT_SOURCE_DIR}/include
        CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

set(OSAL_TEST_DEPENDENCIES
        osal
        core
        unit-test-c++)

set(COMPONENTS_OSAL_TEST_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${OSAL_TEST_DEPENDENCIES}
        CACHE INTERNAL "${PROJECT_NAME}: Link libararies" FORCE)

file(GLOB_RECURSE COMPONENTS_OSAL_TEST_INCLUDES include/*.h)
file(GLOB_RECURSE COMPONENTS_OSAL_TEST_SOURCES src/*.cpp)
file(GLOB_RECURSE COMPONENTS_OSAL_TEST_SOURCES_TEST src/test/*.cpp)

set(COMPONENTS_OSAL_TEST_SOURCES
        ${COMPONENTS_OSAL_TEST_SOURCES}
        ${COMPONENTS_OSAL_TEST_SOURCES_TEST}
        ${COMPONENTS_OSAL_TEST_INCLUDES}
        CACHE INTERNAL "${PROJECT_NAME}: Source files" FORCE)

MESSAGE(STATUS "CMAKE_CXX_FLAGS:                    " ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG:              " ${CMAKE_CXX_FLAGS_DEBUG} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE:            " ${CMAKE_CXX_FLAGS_RELEASE} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:         " ${CMAKE_CXX_FLAGS_MINSIZEREL} )
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:     " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} )
list_to_string(COMPONENTS_OSAL_TEST_INCLUDE STR)
MESSAGE(STATUS "Include files:                      " ${STR} )
list_to_string(COMPONENTS_OSAL_TEST_SOURCES STR)
MESSAGE(STATUS "Source files:                       " ${STR} )
list_to_string(COMPONENTS_OSAL_TEST_LIBS STR)
MESSAGE(STATUS "Link libraries:                     " ${STR} )

# add the executable
add_executable(${PROJECT_NAME} ${COMPONENTS_OSAL_TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} ${COMPONENTS_OSAL_TEST_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMPONENTS_OSAL_TEST_INCLUDE})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPONENTS_OSAL_TEST_DEFINITIONS})
setup_target_properties_executable()

show_target_properties()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
