#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "colors.hpp"
#include "core.hpp"
#include "file.hpp"


const char* concat(const std::string& a, const std::string& b) {
  return (a + b).c_str();
}

std::vector<fs::directory_entry> get_command_files() {
  const std::string path = commands_dir.string();
  std::vector<fs::directory_entry> directories;
  for (const auto& entry : fs::directory_iterator(path)) {
    directories.push_back(entry);
  }
  return directories;
}

void show_files() {
  get_command_files();
  std::cout << BLUE "Your commands (" << commands.size() << ")"
            << ": " << RESET << std::endl;
  size_t i = 1;
  for (const std::string& com : commands) {
    std::cout << YELLOW << i++ << "." << com << RESET << std::endl;
  }
}

void create_command() {
  std::cout << GREEN "Enter file name: " RESET << std::endl;

  std::string filename;
  getline(std::cin, filename);

  const char* path = concat(COMMAND_DIR, concat(filename, ".bat"));
  std::cout << path << '\n';
  FILE *file = std::fopen(path, "w");
  if (!file) {
    fprintf(stderr, RED "Cannot open file!\n" RESET);
    return;
  }
  std::cout << MAGENTA "File editor mode" RESET << std::endl;

  std::string content;
  while (true) {
    std::cout << ": ";
    std::string line;
    getline(std::cin, line);
    if (line == ".exit") {
      break;
    }
    if (line == ".save") {
      save_file(filename, file, content);
      std::cout << "After save\n";
      return;
    }

    // append to file
    line += '\n';
    for (const char c : line) {
      if ((c < 126 && c >= 32) || c == 10) {
        content += c;
      }
    }
  }
  fclose(file);
}

void open_prompt() {
  while (true) {
    std::cout << "What would you like to do?\n"
                 "1. Create new command\n"
                 "2. See all created commands\n"
                 "3. type \".exit\" or \"3\" to leave\n";

    std::string input;
    getline(std::cin, input);
    if (input == ".exit" || input == "3") {
      break;
    }
    if (input == "1") {
      create_command();
      continue;
    }
    if (input == "2") {
      show_files();
      continue;
    }
  }
}

int main() {
  open_prompt();
  return 0;
}
