// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include <ftxui/component/component.hpp>
#include <iostream>
#include <ostream>
#include <string>

using namespace ftxui;

MenuEntryOption Colored(ftxui::Color c) {
    MenuEntryOption option;
    option.animated_colors.foreground.enabled = true;
    option.animated_colors.background.enabled = true;
    option.animated_colors.background.active = c;
    option.animated_colors.background.inactive = Color::Black;
    option.animated_colors.foreground.active = Color::White;
    option.animated_colors.foreground.inactive = c;
    return option;
}

int main() {

    std::string password;
    std::cout
        << "What is the password for your user ? (note this is stored locally)"
        << std::endl;
    std::cin >> password;

    auto screen2 = ScreenInteractive::TerminalOutput();
    auto screen = ScreenInteractive::TerminalOutput();
    int selected = 0;
    auto menu = Container::Vertical(
            {
            MenuEntry(" 1. CD", Colored(Color::Red)),
            MenuEntry(" 2. mkdir", Colored(Color::Yellow)),
            MenuEntry(" 3. Install package", Colored(Color::Green)),
            MenuEntry(" 4. rm", Colored(Color::Cyan)),
            MenuEntry(" 5. Other commands (work in progress )",
                    Colored(Color::Cyan)),
            MenuEntry(" 6. quit", Colored(Color::Blue)),
            },
            &selected);

    // Display together the menu with a border
    auto renderer = Renderer(menu, [&] {
            return vbox({
                    hbox(text("selected = "), text(std::to_string(selected))),
                    separator(),
                    menu->Render() | frame,
                    }) |
            border | bgcolor(Color::Black);
            });

    // This is the only thing I need for debugging
    std::cout << "Selected element = " << selected << std::endl;

    auto component = CatchEvent(renderer, [&](Event event) {
            if (event == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
            }
            return false;
            });
    screen.Loop(component);
    try {
        switch (selected) {
            case 0: {
                        std::string dir;
                        std::cout << "Enter directory name to create: ";
                        std::cin >> dir;
                        std::string cmd = "cd " + dir;
                        if (system(cmd.c_str()) != 0)
                            throw std::runtime_error("cd failed.");
                        break;
                    }
            case 1: {
                        std::string dir;
                        std::cout << "Enter directory name to create: ";
                        std::cin >> dir;
                        std::string cmd = "mkdir " + dir;
                        if (system(cmd.c_str()) != 0)
                            throw std::runtime_error("mkdir failed.");
                        break;
                    }
            case 2: {
                        std::string package;
                        std::cout << "Enter the package name you want to install ";
                        std::cin >> package;
                        // Note the space after the password and the -S flag
                        std::string cmd = "echo " + password + " | sudo -S pacman -S " + package +
                            " --noconfirm";
                        system(cmd.c_str());
                        // std::string cmd = "sudo pacman -S " + package + " --noconfirm";
                        if (system(cmd.c_str()) != 0) {
                            throw std::runtime_error("pacman failed");
                        }
                        break;
                    }
            case 3: {
                        std::string directory;
                        std::cout << "Enter the directory you want to remove ?";
                        std::cin >> directory;
                        std::string cmd = "sudo rm -fr " + directory;
                        if (system(cmd.c_str()) != 0) {
                            throw std::runtime_error("rm failed !");
                        }
                        break;
                    }
            case 4: {
                        std::string command;
                        std::cout << "Enter the command you want to input ";
                        std::cin >> command;
                        std::string cmd = command;
                        if (system(cmd.c_str()) != 0) {
                            throw std::runtime_error("Command run failed");
                        }
                        break;
                    }
            case 5: {
                        break;
                        return 0;
                    }
        }
    } catch (const std::exception &e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }
}
