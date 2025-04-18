#include <windows.h>
#include "logger.h"

// For Hiding Console 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    logger log;
    log.log_appdata_folders("appdata_folders.log");

    return 0;
}