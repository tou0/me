#include <stdio.h>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
#endif

void getScreenInfo() {
    #ifdef _WIN32
        // Windows specific code to get screen information
        DISPLAY_DEVICE displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICE);

        for (int i = 0; EnumDisplayDevices(NULL, i, &displayDevice, 0); ++i) {
            printf("Screen %d:\n", i + 1);
            printf("  Device Name: %s\n", displayDevice.DeviceName);
            printf("  Device String: %s\n", displayDevice.DeviceString);
            printf("  State: %s\n", (displayDevice.State & DISPLAY_DEVICE_ACTIVE) ? "Active" : "Inactive");
            printf("  Monitor Name: %s\n", displayDevice.DeviceString);  // Assuming MonitorName and DeviceString are the same
            printf("\n");
        }
    #elif __linux__
        // Linux specific code to get screen information
        FILE* fp = popen("xrandr --query", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running xrandr command on Linux.\n");
            exit(EXIT_FAILURE);
        }

        char line[256];
        while (fgets(line, sizeof(line), fp) != NULL) {
            if (line[0] == ' ' && line[1] == 'c') {
                printf("Screen Information:\n%s", line);
            }
        }

        pclose(fp);
    #elif __APPLE__
        // macOS specific code to get screen information
        FILE* fp = popen("system_profiler SPDisplaysDataType | awk '/Resolution/ {print}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running system_profiler command on macOS.\n");
            exit(EXIT_FAILURE);
        }

        char line[256];
        while (fgets(line, sizeof(line), fp) != NULL) {
            printf("%s", line);
        }

        pclose(fp);
    #endif
}
