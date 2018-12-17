SET(PROJECT_NAME test_time_sync)

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8.3)

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.0)
    cmake_policy(SET CMP0026 OLD)
endif()

SET(CMAKE_VERBOSE_MAKEFILE ON)

############################################################
# Add plain cmake packages
find_package(passive_time_sync REQUIRED
             Boost REQUIRED)

# Set link libraries
LIST(APPEND LINK_LIBS 
    ${passive_time_sync_LIBRARIES}
    ${Boost_LIBRARIES}
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
      ${passive_time_sync_INCLUDE_DIRS}
)

# Add executable called "test_time_sync" that is built from the source files
# "test_time_sync.cpp". The extensions are automatically found.
add_executable (test_time_sync test_time_sync.cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(test_time_sync ${LINK_LIBS})
