//
// Created by Student on 26.03.2024.
//

#ifndef CORE_HPP
#define CORE_HPP

#define COMMAND_DIR "./commands/"
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

inline std::vector<std::string> commands;
inline fs::path commands_dir = fs::absolute(COMMAND_DIR);

#endif //CORE_HPP
