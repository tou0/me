#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>
#elif __linux__
    #include <sys/sysinfo.h>
#elif __APPLE__
    #include <sys/sysctl.h>
    #include <mach/mach.h>
#endif

void getRAMInfo() {
    #ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(memInfo);
        GlobalMemoryStatusEx(&memInfo);

        printf("RAM Information:\n");
        printf("  Total RAM: %llu MB\n", memInfo.ullTotalPhys / (1024 * 1024));
        printf("  Free RAM: %llu MB\n", memInfo.ullAvailPhys / (1024 * 1024));

        // Add more details as needed
    #elif __linux__
        struct sysinfo info;
        sysinfo(&info);

        printf("RAM Information:\n");
        printf("  Total RAM: %ld MB\n", info.totalram / (1024 * 1024));
        printf("  Free RAM: %ld MB\n", info.freeram / (1024 * 1024));

        // Add more details as needed
    #elif __APPLE__
        mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
        vm_statistics_data_t vmstat;

        host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count);

        printf("RAM Information:\n");
        printf("  Total RAM: %llu MB\n", (unsigned long long)vmstat.wire_count + vmstat.active_count + vmstat.inactive_count + vmstat.free_count / (1024 * 1024));
        printf("  Free RAM: %llu MB\n", (unsigned long long)vmstat.free_count / (1024 * 1024));

        // Add more details as needed
    #else
        // Unsupported platform
        printf("Unsupported platform for RAM information retrieval.\n");
    #endif
}

