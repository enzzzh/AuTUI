cmake_minimum_required(VERSION 3.14)
project(AuTUI VERSION 1.0.0 LANGUAGES CXX)

# Optimization: Disable unnecessary components of FTXUI
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# Optimization: Fully disconnect FetchContent since we manually cloned
set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(ftxui
    SOURCE_DIR /home/enzo/Documents/Code/AuTUI/build/_deps/ftxui-src
)
FetchContent_MakeAvailable(ftxui)

add_executable(autui src/main.cpp)

target_link_libraries(autui PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)
