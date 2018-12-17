SET(PROJECT_NAME passive_time_sync)

project(${PROJECT_NAME})
cmake_minimum_required(VERSION 2.8.3)

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.0)
    cmake_policy(SET CMP0026 OLD)
endif()

SET(CMAKE_VERBOSE_MAKEFILE ON)

############################################################
# Build type and flags
SET(CMAKE_BUILD_TYPE Release) # Options:  Debug, RelWithDebInfo, Release
SET(CMAKE_CXX_FLAGS "-Wall -Werror -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unknown-pragmas")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3 -mmmx -msse -msse -msse2 -msse3 -mssse3 -fomit-frame-pointer")

# Add plain cmake packages
find_package(Eigen3 REQUIRED)

# Add Sources
SET(SOURCES  ${PROJECT_SOURCE_DIR}/src/passive_time_sync.cpp
)

# Add library
INCLUDE_DIRECTORIES(include/passive_time_sync
                    src
                    ${EIGEN3_INCLUDE_DIRS}
)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCES})

################################################################################
# Create the passive_time_syncConfig.cmake file for other cmake projects.
GET_TARGET_PROPERTY( FULL_LIBRARY_NAME ${PROJECT_NAME} LOCATION )
SET(passive_time_sync_LIBRARIES ${FULL_LIBRARY_NAME} )
SET(passive_time_sync_LIBRARY_DIR ${PROJECT_BINARY_DIR} )
SET(passive_time_sync_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/passive_time_syncConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/passive_time_syncConfig.cmake @ONLY IMMEDIATE )
export( PACKAGE passive_time_sync )

INSTALL(DIRECTORY include/passive_time_sync DESTINATION ${CMAKE_INSTALL_PREFIX}/include FILES_MATCHING PATTERN "*.h" )
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
