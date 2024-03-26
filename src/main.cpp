#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "colors.hpp"

#define COMMAND_DIR "./commands/"

namespace fs = std::filesystem;

std::vector<std::string> commands;
fs::path commands_dir = fs::absolute(COMMAND_DIR);

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

std::string get_env(HKEY hRegistryKey, const std::string& path = "Path") {
  
  BYTE buffer[1024 * 1024];
  DWORD buffsz1 = sizeof(buffer);
  
  RegQueryValueExA(hRegistryKey, path.c_str(), nullptr, nullptr, buffer, std::addressof(buffsz1));
  std::string result = reinterpret_cast<const char*>(buffer);
  return result;
}

void set_env(HKEY hRegistryKey, const std::string& name, const std::string& value) {
  // check is hRegistryKey nullptr
  long code = RegSetValueExA(
    hRegistryKey,
    name.c_str(),
    0,
    REG_EXPAND_SZ,
    LPBYTE(value.c_str()),
    (value.size() + 1) * sizeof(wchar_t)
  );

  if (code != ERROR_SUCCESS) {
    fprintf(stderr, "Failed to set environment variable | Code %ld", code);
    RegCloseKey(hRegistryKey);
    exit(EXIT_FAILURE);
  }
}

void append_path(const std::string& to_append, const std::string& key = "Path") {
  // read
  HKEY read_key;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ, &read_key);
  const std::string value = get_env(read_key, key);
  std::cout << "< " << value << '\n';
  RegCloseKey(read_key);
  // write
  HKEY write_key;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &write_key);
  const std::string written = value + ";" + to_append;
  std::cout << "> " << written << '\n';
  set_env(write_key, key, written);
  RegCloseKey(write_key);
}

void save_file(const std::string &filename, FILE *file, const std::string &content) {
  fputs(content.c_str(), file);
  fclose(file);
  std::cout << "File saved!" << std::endl;
  append_path(commands_dir.string());
  std::cout << "Appended to path: " << commands_dir << '\n';
  commands.push_back(filename);
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
  FILE *file = fopen(path, "w");
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
