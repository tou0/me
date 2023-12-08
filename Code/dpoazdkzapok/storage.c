#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <sys/statvfs.h>
#elif __APPLE__
    #include <sys/mount.h>
#endif

void getStorageInfo() {
    #ifdef _WIN32
        ULARGE_INTEGER totalSize, freeSpace, totalFreeSpace;

        if (GetDiskFreeSpaceEx(NULL, &freeSpace, &totalSize, &totalFreeSpace)) {
            printf("Storage Information:\n");
            printf("  Total Storage: %llu GB\n", totalSize.QuadPart / (1024 * 1024 * 1024));
            printf("  Free Storage: %llu GB\n", totalFreeSpace.QuadPart / (1024 * 1024 * 1024));
        } else {
            printf("Error retrieving storage information on Windows.\n");
        }

        // Add more details as needed
    #elif __linux__
        struct statvfs buffer;

        if (statvfs("/", &buffer) == 0) {
            printf("Storage Information:\n");
            printf("  Total Storage: %lu GB\n", (buffer.f_blocks * buffer.f_frsize) / (1024 * 1024 * 1024));
            printf("  Free Storage: %lu GB\n", (buffer.f_bfree * buffer.f_frsize) / (1024 * 1024 * 1024));
        } else {
            printf("Error retrieving storage information on Linux.\n");
        }

        // Add more details as needed
    #elif __APPLE__
        struct statfs buffer;

        if (statfs("/", &buffer) == 0) {
            printf("Storage Information:\n");
            printf("  Total Storage: %llu GB\n", (buffer.f_blocks * buffer.f_bsize) / (1024 * 1024 * 1024));
            printf("  Free Storage: %llu GB\n", (buffer.f_bfree * buffer.f_bsize) / (1024 * 1024 * 1024));
        } else {
            printf("Error retrieving storage information on macOS.\n");
        }

        // Add more details as needed
    #else
        // Unsupported platform
        printf("Unsupported platform for storage information retrieval.\n");
    #endif
}
