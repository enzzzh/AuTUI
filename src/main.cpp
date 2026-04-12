#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstring>
#include <ftxui/component/component.hpp>
#include <iostream>
#include <limits>
#include <ostream>
#include <string>
#include <unistd.h>

using namespace ftxui;
using namespace std;

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

  string password;
  cout << "What is the password for your user ? (note this is stored locally)"
       << endl;
  getline(cin, password);
  bool quit = false;
  while (!quit) {
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
                 hbox(text("selected = "), text(to_string(selected))),
                 separator(),
                 menu->Render() | frame,
             }) |
             border | bgcolor(Color::Black);
    });

    // This is the only thing I need for debugging
    cout << "Selected element = " << selected << std::endl;

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
        string dir;
        cout << "Enter name of the directory you want to move to: ";
        getline(cin, dir);
        // unused, if chdir don't works
        // string cmd = "cd " + dir;

        if (chdir(dir.c_str()) != 0) {
          throw runtime_error("cd failed " + string(strerror(errno)));
        }

        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
          cout << "It worked, current directory: " << cwd << endl;
        }
        break;
      }
      case 1: {
        string dir;
        cout << "Enter directory name to create: ";

        getline(cin, dir);
        string cmd = "mkdir " + dir;
        if (system(cmd.c_str()) != 0)
          throw runtime_error("mkdir failed.");
        break;
      }
      case 2: {
        string package;
        cout << "Enter the package name you want to install ";

        getline(cin, package);
        string cmd = "echo " + password + " | sudo -S pacman -S " + package +
                     " --noconfirm";
        // system(cmd.c_str());
        if (system(cmd.c_str()) != 0) {
          throw runtime_error("pacman failed");
        }
        break;
      }
      case 3: {
        string directory;
        cout << "Enter the directory you want to remove ?";

        getline(cin, directory);
        string cmd = "sudo rm -fr " + directory;
        if (system(cmd.c_str()) != 0) {
          throw runtime_error("rm failed !");
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        break;
      }
      case 4: {
        string command;
        cout << "Enter the command you want to input ";

        getline(cin, command);
        string cmd = command;
        if (system(cmd.c_str()) != 0) {
          throw runtime_error("Command run failed");
        }
        break;
      }
      case 5: {
        break;
        return 0;
      }
      }
    } catch (const exception &e) {
      cerr << "[Error] " << e.what() << std::endl;
    }
  }
}
