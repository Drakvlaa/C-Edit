#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>
#include <string.h>
void SetUserVariablePath()
{
  HKEY hkey;
  long regOpenResult;
  const char key_name[] = "Environment";
  const char path[] = "D:/custom_command"; // new_value path need to update
  regOpenResult = RegOpenKeyEx(HKEY_CURRENT_USER, key_name, 0, KEY_ALL_ACCESS, &hkey);
  LPCSTR stuff = "Path2"; // Variable Name
  RegSetValueEx(hkey, stuff, 0, REG_SZ, (BYTE *)path, strlen(path));
  RegCloseKey(hkey);
}
void GetUserVariablePath()
{
  static const char path[] = "Path"; // Variable Name
  static BYTE buffer1[1000000];
  DWORD buffsz1 = sizeof(buffer1);
  {
    // HKEY_CURRENT_USER\Environment
    const char key_name[] = "Environment";
    HKEY key;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, key_name, 0, KEY_QUERY_VALUE, std::addressof(key)) == 0 &&
        RegQueryValueExA(key, path, nullptr, nullptr, buffer1, std::addressof(buffsz1)) == 0)
    {
      std::cout << "The updated value of the user variable is :  " << reinterpret_cast<const char *>(buffer1) << '\n';
    }
  }
}
void createCommand()
{
  std::cout << "Enter file name: " << std::endl;
  std::string filename;
  getline(std::cin, filename);
  FILE *file = fopen((filename + ".bat").c_str(), "w");
  std::filesystem::path p = filename;
  std::cout << std::filesystem::absolute(p) << std::endl;

  if (!file)
  {
    fprintf(stderr, "Error!");
    return;
  }
  std::string line;
  std::cout << "\033[35mFile editor mode\033[0m" << std::endl;
  do
  {
    std::cout << ": ";
    getline(std::cin, line);
    if (line == ".save")
    {
      fclose(file);
      std::cout << "File saved!" << std::endl;
      std::string path = std::filesystem::absolute(p).string() + ".bat\"";
      SetUserVariablePath();
      exit(0);
      // savefile();
    }
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
  GetUserVariablePath();
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
