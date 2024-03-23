#include <iostream>
#include <fstream>
#include <stdio.h>
#include "color.hpp"
void createCommand()
{
  std::cout << "Enter file name: " << std::endl;
  std::string filename;
  getline(std::cin, filename);
  FILE *file = fopen((filename + ".bat").c_str(), "w");
  if (!file)
  {
    fprintf(stderr, "Error!");
    return;
  }
  std::string line;
  std::cout << dye::purple("File editor mode") << std::endl;
  do
  {
    std::cout << ": ";
    getline(std::cin, line);
    line += '\n';
    std::cout << line << std::endl;
    if (line == ".save"){
      fclose(file);
      exit(0);
    //savefile();
    }
    for (char c: line){
      if ((c < 126 && c >= 32) || c == 10){
        std::string linestr = "";
        linestr += c; 
        fputs(linestr.c_str(), file);
      }
    }
  } while (line != ".exit");

  fclose(file);
}

int main()
{
  std::cout <<
  "What would you like to do?\n"
  "1. Create new command\n"
  "2. See all created commands\n"
  "3. help\n"
  "4. type \".exit\" to leave\n";

  while (true)
  {
    std::string input;
    getline(std::cin, input);
    if (input == ".exit") break;
    if (input == "1") createCommand();
    if (input == "2") break; // printCommands();
    if (input == "3") break; // help();
  }

  return 0;
}
