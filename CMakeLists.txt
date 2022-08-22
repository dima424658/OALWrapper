cmake_minimum_required (VERSION 3.24)
project (OALWrapper VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-fcolor-diagnostics)
endif()

SET(all_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_AudioData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Effect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Effect_Reverb.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_EffectSlot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_EFX.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_EFXManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Filter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Loaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_LoggerObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_OggSample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_OggStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_WAVSample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_CustomStream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Playback.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Sample.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Source.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_SourceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Stream.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sources/OAL_Types.cpp
)

add_library(OALWrapper STATIC ${all_sources})

find_package(Vorbis REQUIRED)
find_package(OpenAL REQUIRED)

target_link_libraries(OALWrapper Vorbis::vorbisfile OpenAL::OpenAL)
target_include_directories(OALWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Tests
add_executable(SimpleTest EXCLUDE_FROM_ALL tests/Simple/main.cpp)
target_link_libraries(SimpleTest OALWrapper)

add_executable(PlaySound EXCLUDE_FROM_ALL tests/PlaySound/main.cpp)
target_link_libraries(PlaySound OALWrapper)

add_executable(Playlist EXCLUDE_FROM_ALL tests/Playlist/main.cpp)
target_link_libraries(Playlist OALWrapper)

add_executable(CustomStream EXCLUDE_FROM_ALL tests/CustomStream/main.cpp)
target_link_libraries(CustomStream OALWrapper)

# Exports
#SET(OALWRAPPER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
