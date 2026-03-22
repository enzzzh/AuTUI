cmake_minimum_required(VERSION 3.14)
project(AuTUI VERSION 1.0.0 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v6.1.9 
)
FetchContent_MakeAvailable(ftxui)

add_executable(autui src/main.cpp)

target_link_libraries(autui PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)

