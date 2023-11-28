cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(GhostLiner)
set(PROJECT_TARGET ${PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
  GIT_TAG 5.0)

FetchContent_Declare(
  muli
  GIT_REPOSITORY "https://github.com/Sopiro/Muli.git"
  GIT_TAG master)

set(MULI_BUILD_DEMO OFF)
FetchContent_MakeAvailable(raylib muli)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")

if(EMSCRIPTEN)
  set(WEB_FLAGS
      "--preload-file ${CMAKE_SOURCE_DIR}/assets@assets/ -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY"
  )
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WEB_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WEB_FLAGS}")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  file(COPY "${CMAKE_SOURCE_DIR}/HTML5Runner.html"
       DESTINATION "${CMAKE_BINARY_DIR}/index.html")
endif()

set(SOURCES "src/main.cpp" "src/gsm.cpp" "src/thing.cpp" "src/camera.cpp")

set(TEMPLATES)

set(HEADERS "src/gsm.hpp" "src/thing.hpp" "src/camera.hpp")

add_executable(${PROJECT_TARGET} ${SOURCES} ${TEMPLATES} ${HEADERS})
target_link_libraries(${PROJECT_TARGET} raylib muli)