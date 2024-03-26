#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "colors.hpp"

std::vector<std::string> commands;

const char* concat(const std::string& a, const std::string& b) {
  return (a + b).c_str();
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
    (LPBYTE) value.c_str(),
    (value.size() + 1) * sizeof(wchar_t)
  );
  
  if (code != ERROR_SUCCESS) {
    fprintf(stderr, "Failed to set environment variable | Code %ld", code);
    RegCloseKey(hRegistryKey);
    exit(EXIT_FAILURE);
  }
}

void append_path(const std::string& to_append, const std::string& key = "Path") {
  std::string value;
  // read
  HKEY read_key;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_READ, &read_key);
  value = get_env(read_key, key);
  std::cout << "< " << value << '\n';
  RegCloseKey(read_key);
  // write
  HKEY write_key;
  RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_ALL_ACCESS, &write_key);
  std::string written = value + ";" + to_append;
  std::cout << "> " << written << '\n';
  set_env(write_key, key, written);
  RegCloseKey(write_key);
}

void save_file(const std::string &filename, FILE *file, const std::string &content) {
  fputs(content.c_str(), file);
  fclose(file);
  std::cout << "File saved!" << std::endl;
  std::filesystem::path abs_path = std::filesystem::absolute(filename);
  append_path(abs_path.string());
  std::cout << "Appended to path: " << abs_path << '\n';
  commands.push_back(filename);
}

void show_files() {
  std::cout << BLUE "Your commands (" << commands.size() << ")"
            << ": " << RESET << std::endl;
  size_t i = 1;
  for (std::string com : commands) {
    std::cout << YELLOW << i++ << "." << com << RESET << std::endl;
  }
}

void create_command() {
  std::cout << GREEN "Enter file name: " RESET << std::endl;

  std::string filename;
  getline(std::cin, filename);

  FILE *file = fopen(concat(filename, ".bat"), "w");
  if (!file) {
    fprintf(stderr, RED "Cannot open file!" RESET);
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
    for (char c : line) {
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
