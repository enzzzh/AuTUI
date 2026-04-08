cmake_minimum_required(VERSION 3.14)
project(AuTUI VERSION 1.0.0 LANGUAGES CXX)

# Optimization: Disable unnecessary components of FTXUI
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v5.0.0
  GIT_SHALLOW TRUE
  FETCHCONTENT_QUIET OFF
)
FetchContent_MakeAvailable(ftxui)

add_executable(autui src/main.cpp)

target_link_libraries(autui PRIVATE
  ftxui::screen
  ftxui::dom
  ftxui::component
)
