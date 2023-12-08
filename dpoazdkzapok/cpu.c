#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    // Include Windows-specific headers
    #include <Windows.h>
    #include <WinBase.h>
    #include <intrin.h>
#elif __linux__
    // Include Linux-specific headers
    #include <sys/sysinfo.h>
    #include <unistd.h>
#elif __APPLE__
    // Include macOS-specific headers
    #include <sys/sysctl.h>
#endif

void getCPUInfo() {
    #ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        printf("CPU Information:\n");
        printf("  Architecture: x86\n");
        printf("  Number of Processors: %u\n", sysInfo.dwNumberOfProcessors);
        printf("  Processor Type: %u\n", sysInfo.dwProcessorType);
        printf("  Page Size: %u bytes\n", sysInfo.dwPageSize);

        // Add more details as needed
    #elif __linux__
        struct sysinfo info;
        sysinfo(&info);

        printf("CPU Information:\n");
        printf("  Number of Processors: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
        printf("  CPU Type: %s\n", "x86");  // Assuming x86 for simplicity

        // Add more details as needed
    #elif __APPLE__
        size_t len = sizeof(int);
        int cpuType;

        sysctlbyname("hw.cputype", &cpuType, &len, NULL, 0);

        printf("CPU Information:\n");
        printf("  Number of Processors: %d\n", (int)sysconf(_SC_NPROCESSORS_ONLN));
        printf("  CPU Type: %s\n", "x86");  // Assuming x86 for simplicity

        // Add more details as needed
    #else
        // Unsupported platform
        printf("Unsupported platform for CPU information retrieval.\n");
    #endif
}
