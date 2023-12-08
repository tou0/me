#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Include this line for usleep

#ifdef _WIN32
    #include <Windows.h>
    #include <pdh.h>
    #include <pdhmsg.h>
#elif __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/sysinfo.h>
#elif __APPLE__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/sysctl.h>
#endif

// Function to get CPU usage percentage
float getCPUUsage() {
    #ifdef _WIN32
        PDH_HQUERY cpuQuery;
        PDH_HCOUNTER cpuTotal;
        PDH_FMT_COUNTERVALUE counterVal;

        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        PdhCollectQueryData(cpuQuery);
        PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

        PdhCloseQuery(cpuQuery);

        return counterVal.doubleValue;
#elif __linux__
    FILE* fp = popen("mpstat 1 1 | awk '$12 ~ /[0-9.]+/ { print 100 - $12 }'", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error running mpstat command on Linux.\n");
        exit(EXIT_FAILURE);
    }

    float cpuUsage;
    fscanf(fp, "%f", &cpuUsage);

    pclose(fp);

    return cpuUsage;
    #elif __APPLE__
        FILE* fp = popen("top -l 1 -n 0 | awk '/CPU usage:/ {print $3}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running top command on macOS.\n");
            exit(EXIT_FAILURE);
        }

        float cpuUsage;
        fscanf(fp, "%f", &cpuUsage);

        pclose(fp);

        return cpuUsage;
    #endif
}

// Function to get RAM usage percentage
float getRAMUsage() {
    #ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

        float totalMemory = memInfo.ullTotalPhys / (1024.0 * 1024.0);  // Convert to MB
        float usedMemory = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024.0 * 1024.0);  // Convert to MB

        return (usedMemory / totalMemory) * 100.0;
    #elif __linux__
        struct sysinfo info;
        sysinfo(&info);

        float totalMemory = info.totalram * info.mem_unit / (1024.0 * 1024.0);  // Convert to MB
        float usedMemory = (info.totalram - info.freeram) * info.mem_unit / (1024.0 * 1024.0);  // Convert to MB

        return (usedMemory / totalMemory) * 100.0;
    #elif __APPLE__
        FILE* fp = popen("top -l 1 -n 0 | awk '/PhysMem:/ {print $8}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running top command on macOS.\n");
            exit(EXIT_FAILURE);
        }

        float usedMemory;
        fscanf(fp, "%f", &usedMemory);

        pclose(fp);

        return usedMemory;
    #endif
}

// Function to get storage usage percentage
float getStorageUsage() {
    #ifdef _WIN32
        // Windows specific code to get storage information
        // You may use appropriate Windows API calls to get storage usage
        // Placeholder value used for simplicity
        return 50.0;
    #elif __linux__
        FILE* fp = popen("df -h --output=pcent / | awk 'NR==2 {print substr($0, 1, length-1)}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running df command on Linux.\n");
            exit(EXIT_FAILURE);
        }

        float storageUsage;
        fscanf(fp, "%f", &storageUsage);

        pclose(fp);

        return storageUsage;
    #elif __APPLE__
        FILE* fp = popen("df -h / | awk 'NR==2 {print substr($0, length($0)-2)}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running df command on macOS.\n");
            exit(EXIT_FAILURE);
        }

        float storageUsage;
        fscanf(fp, "%f", &storageUsage);

        pclose(fp);

        return storageUsage;
    #endif
}

// Function to get GPU usage percentage
float getGPUUsage() {
    #ifdef _WIN32
        HQUERY query;
        HCOUNTER counter;
        PDH_FMT_COUNTERVALUE counterVal;

        // Initialize PDH library
        if (PdhOpenQuery(NULL, 0, &query) != ERROR_SUCCESS) {
            fprintf(stderr, "Error opening PDH query.\n");
            exit(EXIT_FAILURE);
        }

        // Add GPU usage counter (NVIDIA specific)
        if (PdhAddCounter(query, "\\GPU Engine(_Total)\\Utilization", 0, &counter) != ERROR_SUCCESS) {
            fprintf(stderr, "Error adding counter to PDH query.\n");
            PdhCloseQuery(query);
            exit(EXIT_FAILURE);
        }

        // Collect data
        if (PdhCollectQueryData(query) != ERROR_SUCCESS) {
            fprintf(stderr, "Error collecting data from PDH query.\n");
            PdhCloseQuery(query);
            exit(EXIT_FAILURE);
        }

        // Sleep for a short time
        Sleep(1000);

        // Collect data again
        if (PdhCollectQueryData(query) != ERROR_SUCCESS) {
            fprintf(stderr, "Error collecting data from PDH query.\n");
            PdhCloseQuery(query);
            exit(EXIT_FAILURE);
        }

        // Get formatted counter value
        if (PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &counterVal) != ERROR_SUCCESS) {
            fprintf(stderr, "Error getting formatted counter value.\n");
            PdhCloseQuery(query);
            exit(EXIT_FAILURE);
        }

        // Close PDH query
        PdhCloseQuery(query);

        return (float)counterVal.doubleValue;
    #elif __linux__
        FILE* fp = popen("nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader,nounits", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running nvidia-smi command on Linux.\n");
            exit(EXIT_FAILURE);
        }

        float gpuUsage;
        fscanf(fp, "%f", &gpuUsage);

        pclose(fp);

        return gpuUsage;
    #elif __APPLE__
        FILE* fp = popen("system_profiler SPDisplaysDataType | awk '/Chipset Model/ {getline; print substr($0, length($0)-2)}'", "r");
        if (fp == NULL) {
            fprintf(stderr, "Error running system_profiler command on macOS.\n");
            exit(EXIT_FAILURE);
        }

        float gpuUsage;
        fscanf(fp, "%f", &gpuUsage);

        pclose(fp);

        return gpuUsage;
    #else
        // Placeholder value used for non-supported platforms
        return 0.0;
    #endif
}

void getUsagePercentage() {
    // Get and display usage percentage for each hardware component
    float cpuUsage = getCPUUsage();
    printf("CPU Usage: %.2f%%\n", cpuUsage);

    float ramUsage = getRAMUsage();
    printf("RAM Usage: %.2f%%\n", ramUsage);

    float storageUsage = getStorageUsage();
    printf("Storage Usage: %.2f%%\n", storageUsage);

    float gpuUsage = getGPUUsage();
    printf("GPU Usage: %.2f%%\n", gpuUsage);
}
