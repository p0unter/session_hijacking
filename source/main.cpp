#include <iostream>
#include <vector>
#include <windows.h>
#include "logger.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    logger log;
    std::vector<std::string> folders = log.log_appdata_folders();

    if (folders.empty()) {
        MessageBox(NULL, "No folders found.", "Info", MB_OK | MB_ICONINFORMATION);
    } else {
        std::string message;
        for (const auto& folder : folders) {
            message += folder + "\n";
        }
        MessageBox(NULL, message.c_str(), "Found Folders", MB_OK | MB_ICONINFORMATION);
    }

    return 0;
}