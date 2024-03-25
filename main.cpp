#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>
#include <string.h>
#include <sstream>
#include "colors.hpp"
const char *add_char(const char *a, const char *b)
{
  char buffer[256];
  strncpy(buffer, a, sizeof(buffer));
  strncat(buffer, b, sizeof(buffer));
  const char *c = buffer;
  return c;
}
const char *GetUserVariablePath()
{
  static const char path[] = "Path";
  static BYTE buffer1[1000000];
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
  LPCSTR stuff = "Path3";
  RegSetValueEx(hkey, stuff, 0, REG_SZ, (BYTE *)path, strlen(path));
  RegCloseKey(hkey);
}
void SaveFile(std::string filename, FILE* file)
{
  std::filesystem::path p = filename;
  SetUserVariablePath(filename.c_str());
  fclose(file);
  std::cout << "File saved!" << std::endl;
  std::string path = std::filesystem::absolute(p).string();
  exit(0);
}
void createCommand()
{
  std::cout << "Enter file name: " << std::endl;
  std::string filename;
  getline(std::cin, filename);
  FILE *file = fopen((filename + ".bat").c_str(), "w");
  std::cout << file << std::endl;
  std::filesystem::path p = filename;
  // std::cout << std::filesystem::absolute(p) << std::endl;

  if (!file)
  {
    fprintf(stderr, "Error!");
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

int main()
{
  DIR *dir = opendir(".");
  std::cout << dir << std::endl;
  std::cout << "What would you like to do?\n"
               "1. Create new command\n"
               "2. See all created commands\n"
               "3. help\n"
               "4. type \".exit\" to leave\n";

  while (true)
  {
    std::string input;
    getline(std::cin, input);
    if (input == ".exit")
      break;
    if (input == "1")
      createCommand();
    if (input == "2")
      break; // printCommands();
    if (input == "3")
      break; // help();
  }

  return 0;
}
