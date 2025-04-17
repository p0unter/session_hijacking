#include <iostream>
#include "logger.h"

int main() {
    /* Hiding Console */
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE);

    logger log;
    log.log_appdata_folders("appdata_folders.log");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}