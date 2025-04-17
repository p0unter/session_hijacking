#include "logger.h"

#include <windows.h>
#include <shlobj.h>
#include <fstream>
#include <filesystem>
#include <iostream>

void logger::log_appdata_folders(const std::string& log_file) {
    char path[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        std::ofstream ofs(log_file, std::ios::out | std::ios::trunc);
        if (!ofs.is_open()) {
            std::cerr << "Log err!" << std::endl;
            return;
        }

        std::string exclude_folder = "ExcludedFolderName";

        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory()) {
                std::string folder_name = entry.path().filename().string();

                if (folder_name == exclude_folder) {
                    continue;
                }

                ofs << entry.path().string() << std::endl;

                std::cout << entry.path().string() << std::endl;
            }
        }
        ofs.close();
        std::cout << "Appdata record received!" << std::endl;
    }
    else {
        std::cerr << "No appdata!" << std::endl;
    }
    std::cin.get();
}