cmake_minimum_required(VERSION 2.8)

project(multi-platform C CXX)

include(${CMAKE_SOURCE_DIR}/cmake/config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dump_vars.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/setup_default_options.cmake)

set(LANGUAGE CXX)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

# find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
# find_package(Cppsdk REQUIRED COMPONENTS generics)

if (BUILD_MAJOR)
    set(VERSION_MAJOR ${BUILD_MAJOR})
else()
    set(VERSION_MAJOR 0)
endif()

if (BUILD_MINOR)
    set(VERSION_MINOR ${BUILD_MINOR})
else()
    set(VERSION_MINOR 0)
endif()

if (BUILD_REVISION)
    set(VERSION_REVISION ${BUILD_REVISION})
else()
    set(VERSION_REVISION 0)
endif()

if (BUILD_NUMBER)
    set(VERSION_BUILD ${BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}.${VERSION_BUILD})

if(CMAKE_BUILD_TYPE)
    message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Setting build to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CONFIG_DIR
    $<$<CONFIG:debug>:debug>$<$<CONFIG:release>:release>$<$<CONFIG:minsizerel>:releaseMinSize>$<$<CONFIG:relwithdebinfo>:releaseWithDebug> CACHE STRING
    "Config directory.")
set(TEST_DATA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/${CONFIG_DIR} CACHE STRING
    "Build directory.")
message(STATUS "Build base directory: " ${CMAKE_BINARY_DIR})

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output)
message(STATUS "Output base directory: " ${OUTPUT_BASE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/lib)
message(STATUS "Static lib output directory:  " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/bin)
message(STATUS "Dynamic lib output directory: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/bin)
message(STATUS "Executable output directory:  " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_MACOSX_RPATH ../lib)
set(CMAKE_SKIP_BUILD_RPATH TRUE)
message(STATUS "Install directory: " ${CMAKE_INSTALL_PREFIX})

# uninstall target
# configure_file(
	# "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
	# "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
	# IMMEDIATE @ONLY)

# add_custom_target(uninstall
	# COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)

# install target
# add_custom_target(install_${PROJECT_NAME}
	# make install
	# DEPENDS ${PROJECT_NAME}
	# COMMENT "Installing ${PROJECT_NAME}")

# add cmake custom module directory
# set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(WIN32 AND MSVC)
    set(WIN_MSVC TRUE)

    message(STATUS "Building on Windows with MSVC")
    # Important! do not keep warning 4251 4275 ignored!
    # Possibly switch warning 4505 back on
    set(FLAGS_CXX 
		/Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710 
        /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_CXX_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_CXX_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(FLAGS_C 
		/Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710 
        /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_C_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_C_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(DEFINES _UNICODE UNICODE _CRT_SECURE_NO_WARNINGS)
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(MINGW)
    message(STATUS "Building on Windows with MinGW")
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

	if(CMAKE_USE_PTHREADS_INIT)
		string(CONCAT ${FLAGS_CXX} -pthread)
		string(CONCAT ${FLAGS_C} -pthread)
	endif()

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(LINUX)
    message(STATUS "Building on Linux")
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

	if(CMAKE_USE_PTHREADS_INIT)
		message(STATUS "Adding pthread flags")
		list(APPEND FLAGS_CXX -pthread)
		list(APPEND FLAGS_C -pthread)
	endif()

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(APPLE)
    message(STATUS "Building on OS X")
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

	if(CMAKE_USE_PTHREADS_INIT)
		string(CONCAT ${FLAGS_CXX} -pthread)
		string(CONCAT ${FLAGS_C} -pthread)
	endif()

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

message(STATUS "Compiler " ${CMAKE_CXX_COMPILER_ID})
set(CMAKE_COMPILER_IS_CLANG OFF)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Using Clang")
    set(CMAKE_COMPILER_IS_CLANG ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions (-std=c++11)
else()
	add_definitions (-D__DYNAMIC_LINKAGE__)
endif()

setup_default_options()

# if (WEBBRIDGE_TEST_MEASUREMENTS)
	# set (GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
	# set (GCC_COVERAGE_LINK_FLAGS    "-lgcov")
	# set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	# set (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
# endif (WEBBRIDGE_TEST_MEASUREMENTS)

# map()
	# key(version)
	# val(${WEBBRIDGE_VERSION_MAJOR}.${WEBBRIDGE_VERSION_MINOR}.${WEBBRIDGE_VERSION_REVISION})
	# key(port)
	# val(${WEBBRIDGE_PORT})
	# key(binding)
	# val(${WEBBRIDGE_BINDING})
    # key(ipv6)
    # val(${WEBBRIDGE_IPV6_SUPPORT})
	# key(idletime)
	# val(${WEBBRIDGE_IDLE_TIME})
	# key(persistentpath)
	# val(${WEBBRIDGE_PERSISTENT_PATH})
	# key(datapath)
	# val(${WEBBRIDGE_DATA_PATH})
	# key(systempath)
	# val(${WEBBRIDGE_SYSTEM_PATH})
	# key(proxystubpath)
	# val(${WEBBRIDGE_PROXYSTUB_PATH})
	# kv(redirect "/Service/Controller/UI")
	# key(plugins)
# end()
# get the result
# ans(WEBBRIDGE_CONFIG)


#
# default switch configuration
# map()
  # kv(default WebKitBrowser)
# end()
# ans(WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH)

#
# default switch configuration
# map()
#  key(switch)
# end()
# ans(WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION)

# default configutation
# map()
  # kv(callsign Controller)
  # key(configuration)
# end()
# ans(WEBBRIDGE_PLUGIN_CONTROLLER)


# if (WEBBRIDGE_PLUGIN_WEBKITBROWSER OR WEBBRIDGE_PLUGIN_NETFLIX OR WEBBRIDGE_PLUGIN_WEBKITBROWSER_YOUTUBE)
	#Add ___array___ to the key to force json array creation.
	# map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH} callsigns ___array___)
# endif()

# if (WEBBRIDGE_PLUGIN_WEBSERVER OR WEBBRIDGE_PLUGIN_WEBKITBROWSER)
        #Add ___array___ to the key to force json array creation.
        # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION} resumes ___array___)
# endif()

# if (WEBBRIDGE_PLUGIN_WEBKITBROWSER)
  # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH} callsigns WebKitBrowser)
  # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION} resumes WebKitBrowser)
# endif (WEBBRIDGE_PLUGIN_WEBKITBROWSER)

# if (WEBBRIDGE_PLUGIN_NETFLIX)
  # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH} callsigns Netflix)
# endif (WEBBRIDGE_PLUGIN_NETFLIX)

# if (WEBBRIDGE_PLUGIN_WEBKITBROWSER_YOUTUBE)
  # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH} callsigns YouTube)
# endif (WEBBRIDGE_PLUGIN_WEBKITBROWSER_YOUTUBE)

# if (WEBBRIDGE_PLUGIN_WEBSERVER)
  # map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION} resumes WebServer)
# endif (WEBBRIDGE_PLUGIN_WEBSERVER)

#map_append(${WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION} switch ${WEBBRIDGE_PLUGIN_CONTROLLER_SWITCH})
# map_append(${WEBBRIDGE_PLUGIN_CONTROLLER} configuration ${WEBBRIDGE_PLUGIN_CONTROLLER_CONFIGURATION})
# map_append(${WEBBRIDGE_CONFIG} plugins ${WEBBRIDGE_PLUGIN_CONTROLLER})

# include_directories ("${PROJECT_SOURCE_DIR}")
# include_directories ("${CPPSDK_INCLUDE_DIRS}")
# add_subdirectory (WebBridgeSupport)  

# save webbridge config
# json_write("${CMAKE_CURRENT_LIST_DIR}/Config.json" ${WEBBRIDGE_CONFIG})
# install(FILES Config.json DESTINATION ${WEBBRIDGE_CONFIG_INSTALL_PATH} RENAME config.json)

add_subdirectory(source)  

# dump_vars()
