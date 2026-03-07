cmake_minimum_required(VERSION 3.31)
set(PROJECT_NAME "EZVN")
project(${PROJECT_NAME})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # MP build for MSVC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp include/*.h)
add_executable(${PROJECT_NAME} ${SOURCES})

# copier le dossier resources/ dans le dossier build/
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_CURRENT_BINARY_DIR}/resources)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_compile_options(${PROJECT_NAME} PUBLIC -W3 /external:anglebrackets /external:W0)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) # C++17

#gérer 32 bits et 64 bits
if(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(SDL2_ROOT "${CMAKE_SOURCE_DIR}/third_party/SDL2-2.30.3/x86_64-w64-mingw32")
else()
  set(SDL2_ROOT "${CMAKE_SOURCE_DIR}/third_party/SDL2-2.30.3/i686-w64-mingw32")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_ROOT}/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_ROOT}/lib
)

target_link_libraries(${PROJECT_NAME}
    SDL2
    SDL2main
    SDL2_image
    SDL2_mixer
    SDL2_ttf
)