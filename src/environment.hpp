#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP
#include <iostream>
#include <string>
#include <windows.h>

inline std::string get_env(HKEY hRegistryKey, const std::string& path = "Path") {

    BYTE buffer[1024 * 1024];
    DWORD buffsz1 = sizeof(buffer);

    RegQueryValueExA(hRegistryKey, path.c_str(), nullptr, nullptr, buffer, std::addressof(buffsz1));
    std::string result = reinterpret_cast<const char*>(buffer);
    return result;
}

inline void set_env(const HKEY hRegistryKey, const std::string& name, const std::string& value) {
    // check is hRegistryKey nullptr

    if (const long code = RegSetValueExA(
        hRegistryKey,
        name.c_str(),
        0,
        REG_EXPAND_SZ,
        LPBYTE(value.c_str()),
        (value.size() + 1) * sizeof(wchar_t)
    ); code != ERROR_SUCCESS) {
        fprintf(stderr, "Failed to set environment variable | Code %ld", code);
        RegCloseKey(hRegistryKey);
        exit(EXIT_FAILURE);
    }
}

inline void append_path(const std::string& to_append, const std::string& key = "Path") {
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

#endif //ENVIRONMENT_HPP
