#include "parser.hh"
#include <iomanip>
#include <iostream>

int main(int argc, char *argv[]) {
    Parser p;
    std::cout << "Welcome to this db. Type HELP for a list of commands, "
              << "or QUIT to exit.\n>>> ";
    std::string line;
    while (std::getline(std::cin, line)) {
        if(!p.do_command(line, true)) {
            break;
        }
    }
}
