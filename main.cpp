#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <sys/types.h>
#include <filesystem>
#include <string.h>
#include <sstream>
#include "colors.hpp"
#include <vector>
std::vector<std::string> commands;

void OpenCommandPrompt();

const char *add_char(const std::string &a, const std::string &b)
{
  return (a + b).c_str();
}

const char *GetUserVariablePath()
{
  static const char path[] = "Path";
  static BYTE buffer1[1];
  DWORD buffsz1 = sizeof(buffer1);
  {
    const char key_name[] = "Environment";
    HKEY key;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, key_name, 0, KEY_QUERY_VALUE, std::addressof(key)) == 0 &&
        RegQueryValueExA(key, path, nullptr, nullptr, buffer1, std::addressof(buffsz1)) == 0)
    {
      return reinterpret_cast<const char *>(buffer1);
    }
    return nullptr;
  }
}

void SetUserVariablePath(const char *name)
{
  HKEY hkey;
  long regOpenResult;
  const char key_name[] = "Environment";
  const char *path = add_char(GetUserVariablePath(), name);
  regOpenResult = RegOpenKeyEx(HKEY_CURRENT_USER, key_name, 0, KEY_ALL_ACCESS, &hkey);
  LPCSTR stuff = "Path2";
  RegSetValueEx(hkey, stuff, 0, REG_SZ, (BYTE *)path, strlen(path));
  RegCloseKey(hkey);
}

void SaveFile(std::string filename, FILE *file)
{
  std::filesystem::path p = filename;
  SetUserVariablePath(filename.c_str());
  fclose(file);
  std::cout << "File saved!" << std::endl;
  OpenCommandPrompt();
  commands.push_back(filename);
}

void DisplayFiles(std::vector<std::string> list)
{
  std::cout << BLUE "Your commands: " RESET << std::endl;
  for (std::string com : list)
  {
    int i = 1;
    std::cout << YELLOW << i << "." << com << RESET << std::endl;
    i++;
  }
  OpenCommandPrompt();
}

void createCommand()
{
  std::cout << GREEN "Enter file name: " RESET << std::endl;
  std::string filename;
  getline(std::cin, filename);
  FILE *file = fopen((filename + ".bat").c_str(), "w");
  std::filesystem::path p = filename;

  if (!file)
  {
    fprintf(stderr, RED "Error!" RESET);
    return;
  }
  std::string line;
  std::cout << MAGENTA "File editor mode" RESET << std::endl;
  do
  {
    std::cout << ": ";
    getline(std::cin, line);
    if (line == ".save")
      SaveFile(filename, file);
    line += '\n';
    for (char c : line)
    {
      if ((c < 126 && c >= 32) || c == 10)
      {
        std::string linestr = "";
        linestr += c;
        fputs(linestr.c_str(), file);
      }
    }
  } while (line != ".exit\n");

  fclose(file);
}

void OpenCommandPrompt()
{
  std::cout << "What would you like to do?\n"
               "1. Create new command\n"
               "2. See all created commands\n"
               "3. type \".exit\" or \"3\" to leave\n";

  while (true)
  {
    std::string input;
    getline(std::cin, input);
    if (input == "1")
      createCommand();
    if (input == "2")
      DisplayFiles(commands);
    if (input == ".exit" || "3")
      exit(0);
  }
  return;
}

int main()
{
  OpenCommandPrompt();
  return 0;
}
