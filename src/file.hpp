//
// Created by Student on 26.03.2024.
//

#ifndef FILE_HPP
#define FILE_HPP
#include <iostream>
#include <string>

#include "core.hpp"
#include "environment.hpp"

inline void save_file(const std::string &filename, FILE *file, const std::string &content) {
    fputs(content.c_str(), file);
    fclose(file);
    std::cout << "File saved!" << std::endl;
    append_path(commands_dir.string());
    std::cout << "Appended to path: " << commands_dir << '\n';
    commands.push_back(filename);
}


#endif //FILE_HPP
