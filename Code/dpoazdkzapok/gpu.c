#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>
    #include <WinBase.h>
    #include <tchar.h>
    #include <Psapi.h>
#elif __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <dirent.h>
#elif __APPLE__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <IOKit/graphics/IOGraphicsLib.h>
#endif

void getGPUInfo() {
    #ifdef _WIN32
        DISPLAY_DEVICE displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICE);
        DEVMODE devMode;
        devMode.dmSize = sizeof(DEVMODE);

        int deviceIndex = 0;

        printf("GPU Information:\n");

        while (EnumDisplayDevices(NULL, deviceIndex, &displayDevice, 0)) {
            if (EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {
                printf("  GPU %d:\n", deviceIndex + 1);
                printf("    Device Name: %s\n", displayDevice.DeviceString);
                printf("    Resolution: %dx%d\n", devMode.dmPelsWidth, devMode.dmPelsHeight);
                // Add more details as needed
            }

            deviceIndex++;
        }
    #elif __linux__
        FILE *fp;
        char buffer[128];

        printf("GPU Information:\n");

        // Run the lspci command to get GPU information
        fp = popen("lspci | grep VGA", "r");
        if (fp == NULL) {
            printf("Error retrieving GPU information on Linux.\n");
            return;
        }

        while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
            printf("  %s", buffer);
            // Add more details as needed
        }

        pclose(fp);
    #elif __APPLE__
        io_iterator_t iterator;
        io_service_t service;
        CFMutableDictionaryRef serviceProperties;

        printf("GPU Information:\n");

        // Create a matching dictionary for IOServiceMatching
        serviceProperties = IOServiceMatching("IOPCIDevice");
        if (serviceProperties == NULL) {
            printf("Error creating a matching dictionary for GPU information on macOS.\n");
            return;
        }

        // Create an iterator across matching devices
        kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, serviceProperties, &iterator);
        if (kr != KERN_SUCCESS) {
            printf("Error getting matching services for GPU information on macOS.\n");
            return;
        }

        // Iterate over matching services
        while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
            CFStringRef gpuName = NULL;
            CFNumberRef gpuVRAM = NULL;

            // Get GPU name
            IORegistryEntryCreateCFProperties(service, &serviceProperties, kCFAllocatorDefault, 0);
            gpuName = CFDictionaryGetValue(serviceProperties, CFSTR("IOName"));

            // Get VRAM size
            gpuVRAM = CFDictionaryGetValue(serviceProperties, CFSTR("VRAM,totalMB"));

            if (gpuName != NULL && gpuVRAM != NULL) {
                char gpuNameStr[256];
                CFStringGetCString(gpuName, gpuNameStr, sizeof(gpuNameStr), kCFStringEncodingUTF8);

                int vramSize;
                CFNumberGetValue(gpuVRAM, kCFNumberIntType, &vramSize);

                printf("  GPU:\n");
                printf("    Device Name: %s\n", gpuNameStr);
                printf("    VRAM Size: %d MB\n", vramSize);
                // Add more details as needed
            }

            IOObjectRelease(service);
        }

        IOObjectRelease(iterator);
    #else
        // Unsupported platform
        printf("Unsupported platform for GPU information retrieval.\n");
    #endif
}


