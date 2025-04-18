/* GCC Compile Command
g++ -Iproc main.cpp proc/logger.cpp -o output/app -mwindows
*/

#include <iostream>
#include <vector>
#include <windows.h>
#include "logger.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    logger log;
    std::vector<std::string> folders = log.log_appdata_folders();

    for (const auto& folder : folders) {
        std::cout << "Found folder: " << folder << std::endl;
    }

    return 0;
}
